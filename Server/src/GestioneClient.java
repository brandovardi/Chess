import java.io.*;
import java.net.*;

public class GestioneClient implements Runnable {
    private Socket clientSocket;
    private Socket opponentSocket;
    private String color;
    private Chess game;

    public GestioneClient() {
        this.clientSocket = null;
        this.opponentSocket = null;
        this.color = null;
        this.game = null;
    }

    public GestioneClient(Socket socket, String color) {
        this.clientSocket = socket;
        this.opponentSocket = null;
        this.color = color;
        this.game = new Chess();
    }

    public void SetOpponentSocket(Socket opponent) {
        this.opponentSocket = opponent;
    }
    
    public void SetGame(Chess game) {
        synchronized (this.game)
        {
            this.game = game;
        }
    }

    public void SendMoveToOpponent(int rstart, int cstart, int rend, int cend, String namePromoted, String endMatch) throws IOException
    {
        String rispostaServer = (endMatch != null ? endMatch : "");
        // dati da inviare all'avversario nel caso la mossa è corretta
        DataOutputStream outOpponent = new DataOutputStream(opponentSocket.getOutputStream());
        Posizione p1 = new Posizione(rstart, cstart);
        Posizione pp1 = this.game.calcolaCoordinateOpposte(p1);

        Posizione p2 = new Posizione(rend, cend);
        Posizione pp2 = this.game.calcolaCoordinateOpposte(p2);

        if (!rispostaServer.equals(""))
        {
            outOpponent.writeBytes(rispostaServer);
            return;
        }
        // mi salvo la mossa con le coordinate del pezzo
        rispostaServer = pp1.riga + ";" + pp1.colonna + ";" + pp2.riga + ";" + pp2.colonna;

        if (namePromoted.equals(""))
            rispostaServer = "OK;" + rispostaServer;
        else
            rispostaServer = namePromoted + ";"+ rispostaServer;

        // invio i dati all'avversario se la mossa è corretta
        outOpponent.writeBytes(rispostaServer);
    }

    public void run() {
        try {
            boolean endMatch = false;

            BufferedReader inputClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            DataOutputStream outputServer = new DataOutputStream(clientSocket.getOutputStream());

            String clientMessage = "start";
            Thread.sleep(1000);
            // la prima volta invio al client che è connesso un'altro utente per poter
            // iniziare la partita
            outputServer.writeBytes(clientMessage);
            while ((clientMessage = inputClient.readLine()) != null) {
                System.out.println("Ricevuto dal client: " + clientMessage);
                String serverResponse = "";

                int rStart = 0, cStart = 0, rEnd = 0, cEnd = 0;
                // se ho ricevuto qualcosa
                if (!clientMessage.equals("")) {

                    String[] splitMessage = clientMessage.split("\t");

                    // se non sono state passate mosse dove servono ulteriori controlli (Matto, Stallo, Promozione)
                    if (splitMessage.length == 1)
                    {
                        String[] attributes = splitMessage[0].split(";");
                        String[] values = attributes[0].split("_");
                        String name = values[1];
                        String color = values[0];
                        rStart = Integer.parseInt(attributes[1]);
                        cStart = Integer.parseInt(attributes[2]);
                        rEnd = Integer.parseInt(attributes[3]);
                        cEnd = Integer.parseInt(attributes[4]);

                        synchronized (this.game)
                        {
                            Piece piece = this.game.PieceFromString(name, color);
                            // if (piece != null && game.CheckPieceMove(piece, rStart, cStart, rEnd, cEnd)) {

                            // }
                        }

                        serverResponse = "OK;" + rStart + ";" + cStart + ";" + rEnd + ";" + cEnd;
                    }
                    else if (splitMessage.length == 2)
                    {
                        if (splitMessage[1].equals("Promoted"))
                        {
                            // controllo mossa
                            //...

                            serverResponse = "OK;";
                            // invia la conferma al client che ha fatto la mossa
                            outputServer.writeBytes(serverResponse);

                            // una volta vericata la mossa aspetto che il client mi invia il pezzo promosso
                            while ((clientMessage = inputClient.readLine()) != null)
                            {
                                System.out.println("Ricevuto dal client: " + clientMessage);
                                // ricevo = "colorePezzo_nomePezzo;rstart;cstart;rend;cend"
                                String[] attributes = clientMessage.split(";");
                                rStart = Integer.parseInt(attributes[1]);
                                cStart = Integer.parseInt(attributes[2]);
                                rEnd = Integer.parseInt(attributes[3]);
                                cEnd = Integer.parseInt(attributes[4]);

                                serverResponse = attributes[0] + ";" + rStart + ";" + cStart + ";" + rEnd + ";" + cEnd;
                                // invia la conferma al client che ha fatto la mossa
                                outputServer.writeBytes(serverResponse);
                                
                                this.SendMoveToOpponent(rStart, cStart, rEnd, cEnd, attributes[0], null);
                                // esco da questo ciclo
                                break;
                            }
                        }
                        else if (splitMessage[1].equals("Checkmate"))
                        {
                            if (clientMessage.contains("white"))
                                serverResponse = "Checkmate;white";
                            else if (clientMessage.contains("black"))
                                serverResponse = "Checkmate;black";
                            endMatch = true;
                        }
                        else if (splitMessage[1].equals("Stealmate"))
                        {
                            serverResponse = "Stealmate;";
                            endMatch = true;
                        }
                    }
                }

                // invia la conferma al client che ha fatto la mossa
                outputServer.writeBytes(serverResponse);

                this.SendMoveToOpponent(rStart, cStart, rEnd, cEnd, "", (endMatch ? serverResponse : null));
            }

            // Se il client chiude la connessione, esci dal loop
            System.out.println("Connessione chiusa da: " + clientSocket.getInetAddress());
            clientSocket.close();

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
