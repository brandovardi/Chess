import java.io.*;
import java.net.*;

public class GestioneClient implements Runnable {
    private Socket clientSocket;
    private Socket opponentSocket;
    private String color;

    public GestioneClient(Socket socket, String color) {
        this.clientSocket = socket;
        this.color = color;
    }

    public void SetOpponentSocket(Socket opponent) {
        this.opponentSocket = opponent;
    }

    public void run() {
        try {
            Chess game = new Chess(color);

            BufferedReader inputClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            DataOutputStream outputServer = new DataOutputStream(clientSocket.getOutputStream());


            String clientMessage = "\n" + "start";
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

                    if (splitMessage.length == 1) {
                        String[] attributes = splitMessage[0].split(";");
                        String[] values = attributes[0].split("_");
                        String name = values[1];
                        String color = values[0];
                        rStart = Integer.parseInt(attributes[1]);
                        cStart = Integer.parseInt(attributes[2]);
                        rEnd = Integer.parseInt(attributes[3]);
                        cEnd = Integer.parseInt(attributes[4]);

                        Piece piece = game.PieceFromString(name, color);
                        // if (piece != null && game.CheckPieceMove(piece, rStart, cStart, rEnd, cEnd)) {

                        // }

                        serverResponse = "OK" + ";" + rStart + ";" + cStart + ";" + rEnd + ";" + cEnd;
                    }
                }

                // serverResponse = "MoveOk";
                // invia la conferma al client che ha fatto la mossa
                outputServer.writeBytes(serverResponse);

                // dati da inviare all'avversario nel caso la mossa è corretta
                DataOutputStream outOpponent = new DataOutputStream(opponentSocket.getOutputStream());
                Posizione p1 = new Posizione(rStart, cStart);
                Posizione pp1 = game.calcolaCoordinateOpposte(p1);

                Posizione p2 = new Posizione(rEnd, cEnd);
                Posizione pp2 = game.calcolaCoordinateOpposte(p2);

                serverResponse = "\nOK" + ";" + pp1.riga + ";" + pp1.colonna + ";" + pp2.riga + ";" + pp2.colonna;
                // invio i dati all'avversario se la mossa è corretta
                outOpponent.writeBytes(serverResponse);
            }

            // Se il client chiude la connessione, esci dal loop
            System.out.println("Connessione chiusa da: " + clientSocket.getInetAddress());
            clientSocket.close();

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
