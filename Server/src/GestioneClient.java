import java.io.*;
import java.net.*;

public class GestioneClient implements Runnable {
    private Socket clientSocket;

    public GestioneClient(Socket socket, ServerSocket serverSocket) {
        this.clientSocket = socket;
    }

    public void run() {
        try {
            Chess game = new Chess(null);

            BufferedReader inFromClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            DataOutputStream outToClient = new DataOutputStream(clientSocket.getOutputStream());

            String clientMessage;
            while ((clientMessage = inFromClient.readLine()) != null) {
                System.out.println("Ricevuto dal client: " + clientMessage);
                String serverResponse = "";

                // se ho ricevuto qualcosa
                if (!clientMessage.equals("")) {
                    
                    String[] splitMessage = clientMessage.split("\t");
    
                    if (splitMessage.length == 1)
                    {
                        String[] attributes = splitMessage[0].split(";");
                        String[] values = attributes[0].split("_");
                        String name = values[1];
                        String color = values[0];
                        int rEnd = Integer.parseInt(attributes[1]);
                        int cEnd = Integer.parseInt(attributes[2]);
                        int rStart = Integer.parseInt(attributes[3]);
                        int cStart = Integer.parseInt(attributes[4]);
    
                        Piece piece = game.PieceFromString(name, color);
                        if (piece != null && game.CheckPieceMove(piece, rStart, cStart, rEnd, cEnd))
                        {
                            
                        }
                    }
                }   

                serverResponse = "Dato ricevuto correttamente: " + clientMessage + "\n";
                // Invia la conferma al client
                outToClient.writeBytes(serverResponse);
            }

            // Se il client chiude la connessione, esci dal loop
            System.out.println("Connessione chiusa da: " + clientSocket.getInetAddress());
            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
