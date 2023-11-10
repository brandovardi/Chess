public class GestioneClient implements Runnable {
    private String posizione;
    

    public GestioneClient() {
        this.posizione = "";
    }

    public GestioneClient(String posizione) {
        this.posizione = posizione;
    }

    public String getPosizione() { return this.posizione; }

    @Override
    public void run() {
        while (true) {
            
        }
    }
}
