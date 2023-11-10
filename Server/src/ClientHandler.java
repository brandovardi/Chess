import java.io.*;
import java.net.*;

public class ClientHandler implements Runnable {
    private Socket clientSocket;

        public ClientHandler(Socket socket) {
            this.clientSocket = socket;
        }

        @Override
        public void run() {
            try {
                BufferedReader inFromClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream());
                DataOutputStream outToClient = new DataOutputStream(clientSocket.getOutputStream());

                String clientMessage;
                while ((clientMessage = inFromClient.readLine()) != null) {
                    System.out.println("Ricevuto dal client " + clientSocket.getInetAddress() + ": " + clientMessage);

                    String rispostaServer = "Dato ricevuto correttamente: " + clientMessage + "\n";
                    // Invia la conferma al client
                    outToClient.writeBytes(rispostaServer);
                }

                System.out.println("Connessione chiusa da: " + clientSocket.getInetAddress());
                clientSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
}
