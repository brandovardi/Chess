import java.io.*;
import java.net.*;
import java.util.*;

public class Server {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(666)) {
            // mi salvo il client precedente e anche la socket
            HashMap<Socket, GestioneClient> savedPlayer = new HashMap<Socket, GestioneClient>();
            
            // il server accetta solamente due giocatori (aventi colori diversi)
            while (savedPlayer.size() < 2) {
                // la prima volta accetto il client
                Socket clientSocket = serverSocket.accept();
                System.out.println("Connessione accettata da: " + clientSocket.getInetAddress());

                savedPlayer.put(clientSocket, new GestioneClient(clientSocket));
            }

            // GestioneClient g1 = savedPlayer.keySet();

            // per ogni giocatore connesso avvio il proprio thread
            // for (GestioneClient g : savedPlayer) {
            //     // avvio il thread
            //     Thread t = new Thread(g);
            //     t.start();
            // }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
