import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(666)) {
            Socket savedClient = null;
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Connessione accettata da: " + clientSocket.getInetAddress());

                if (savedClient != null)
                {

                    // Creazione di un nuovo thread per gestire la connessione
                    GestioneClient g1 = new GestioneClient(clientSocket);
                    g1.SetOpponentSocket(savedClient);
                    Thread clientThread = new Thread(g1);
                    clientThread.start();
                    savedClient = null;

                    break;
                }

                // Creazione di un nuovo thread per gestire la connessione
                Thread clientThread = new Thread(new GestioneClient(clientSocket));
                savedClient = clientSocket;
                clientThread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
