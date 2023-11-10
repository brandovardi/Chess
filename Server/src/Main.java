import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Main {
    public static void main(String[] args) throws Exception {
        // porta d'ascolto
        DatagramSocket socket = new DatagramSocket(666);
        boolean running = false;

        Chess game = new Chess(null);

        // ciclo
        while (!running) {
            // buffer per leggere la richiesta del client
            byte[] buff = new byte[1500];
            DatagramPacket packet = new DatagramPacket(buff, buff.length);
            // devo digli di aspettare un pacchetto
            socket.receive(packet);
            // salvo i dati del client in una stringa (togliendo tutti i caratteri null)
            String ricevuto = new String(packet.getData(), 0, packet.getLength());
            System.out.println(ricevuto);

            // messaggio di risposta del server
            String messaggio = "";

            // se ho ricevuto qualcosa
            if (!ricevuto.equals("")) {
                
                String[] splitMessage = ricevuto.split("\n");

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

            // contiene l'inetaddress del client
            InetAddress ipClient = packet.getAddress();
            // porta contenuta nel pacchetto ricevuto dal client
            int portClient = packet.getPort();
            // creo un buffer da inviare con il messaggio di risposta del server
            buff = messaggio.getBytes();
            DatagramPacket p2 = new DatagramPacket(buff, buff.length);
            p2.setAddress(ipClient);
            p2.setPort(portClient);
            socket.send(p2);
        }

        // chiudo la socket
        socket.close();
    }
}
