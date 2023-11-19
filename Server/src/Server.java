import java.io.*;
import java.net.*;
import java.util.*;

public class Server {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(666)) {
            // mi salvo il client precedente e anche la socket
            HashMap<Socket, GestioneClient> savedPlayer = new HashMap<Socket, GestioneClient>();
            String firstColor = null;
            String secondColor = null;

            // il server accetta solamente due giocatori (aventi colori diversi)
            while (savedPlayer.size() < 2) {
                // la prima volta accetto il client
                Socket clientSocket = serverSocket.accept();
                System.out.println("Connessione accettata da: " + clientSocket.getInetAddress());

                if (firstColor == null)
                { // se non si ancora connesso nessun client allora il primoColore sarà nullo quindi lo inizializzo
                    BufferedReader inputClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                    firstColor = inputClient.readLine(); // qua leggo il colore che mi invia il client
                    savedPlayer.put(clientSocket, new GestioneClient(clientSocket, firstColor));
                }
                else
                { // ora controllo se il secondo client connesso è di colore diverso del primo altrimenti chiudo la connessione
                    BufferedReader inputClient = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                    secondColor = inputClient.readLine(); // qua leggo il colore che mi invia il client
                    if (secondColor.equals("black") && firstColor.equals("white") || secondColor.equals("white") && firstColor.equals("black")) {
                                 // se il colore è diverso allora lo aggiungo alla partita
                        savedPlayer.put(clientSocket, new GestioneClient(clientSocket, secondColor));
                    } else { // altrimenti chiudo la socket in attesa di un'altro giocatore
                        clientSocket.close();
                        inputClient.close();
                    }
                }
            }
            // creo un campo generale per tutti e due i client
            Chess game = new Chess(null);

            boolean count = false;
            // per ogni giocatore connesso avvio il proprio thread
            for (GestioneClient g : savedPlayer.values()) {
                // scorro i due giocatori e gli assegno il proprio avversario (indicandogli la
                // socket)
                for (Socket sock : savedPlayer.keySet()) {
                    if (count) {
                        g.SetOpponentSocket(sock);
                        g.SetGame(game);
                        break;
                    }
                    count = true;
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
