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
            boolean count = false;
            // per ogni giocatore connesso avvio il proprio thread
            for (GestioneClient g : savedPlayer.values()) {
                // scorro i due giocatori e gli assegno il proprio avversario (indicandogli la socket)
                for (Socket sock : savedPlayer.keySet()) {
                    if (!count)
                    {
                        // siccome so che ci sono solamente due giocatori, al primo ciclo cambio il valore della variabile
                        // per il ciclo successivo
                        count = true;
                        continue;
                    }
                    else
                    {
                        g.SetOpponentSocket(sock);
                    }
                }
                // avvio i thread per entrambi i giocatori
                Thread t = new Thread(g);
                t.start();
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
