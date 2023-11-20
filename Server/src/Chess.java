class Posizione {
    public int riga;
    public int colonna;

    Posizione(int riga, int colonna) {
        this.riga = riga;
        this.colonna = colonna;
    }
}

public class Chess {
    // costanti
    final public int ROW = 8;
    final public int COLUMN = 8;
    
    // attributi
    private Piece[][] chessBoard;
    private Piece.Color playerColor;
    private Piece.Color opponentColor;
    private boolean whiteToMove;

    public Chess() {
        this.whiteToMove = true;
        this.chessBoard = new Piece[ROW][COLUMN];
        // siccome utilizzo solamente una scacchiera metto i pezzi bianchi sempre in basso
        this.playerColor = Piece.Color.WHITE;
        this.opponentColor = Piece.Color.BLACK;

        // all pieces
        this.chessBoard[7][0] = new Piece(Piece.Name.ROOK, this.playerColor);
        this.chessBoard[7][1] = new Piece(Piece.Name.KNIGHT, this.playerColor);
        this.chessBoard[7][2] = new Piece(Piece.Name.BISHOP, this.playerColor);
        this.chessBoard[7][5] = new Piece(Piece.Name.BISHOP, this.playerColor);
        this.chessBoard[7][6] = new Piece(Piece.Name.KNIGHT, this.playerColor);
        this.chessBoard[7][7] = new Piece(Piece.Name.ROOK, this.playerColor);
        this.chessBoard[7][3] = new Piece(Piece.Name.QUEEN, this.opponentColor);
        this.chessBoard[7][4] = new Piece(Piece.Name.KING, this.opponentColor);
        
        this.chessBoard[0][0] = new Piece(Piece.Name.ROOK, this.opponentColor);
        this.chessBoard[0][1] = new Piece(Piece.Name.KNIGHT, this.opponentColor);
        this.chessBoard[0][2] = new Piece(Piece.Name.BISHOP, this.opponentColor);
        this.chessBoard[0][5] = new Piece(Piece.Name.BISHOP, this.opponentColor);
        this.chessBoard[0][6] = new Piece(Piece.Name.KNIGHT, this.opponentColor);
        this.chessBoard[0][7] = new Piece(Piece.Name.ROOK, this.opponentColor);
        this.chessBoard[0][3] = new Piece(Piece.Name.QUEEN, this.opponentColor);
        this.chessBoard[0][4] = new Piece(Piece.Name.KING, this.opponentColor);

        // all pawns
        for (int j = 0; j < COLUMN; j++) {
            this.chessBoard[1][j] = new Piece(Piece.Name.PAWN, opponentColor);
            this.chessBoard[6][j] = new Piece(Piece.Name.PAWN, playerColor);
        }
    }

    public boolean CheckPieceMove(Piece piece, int rStart, int cStart, int rEnd, int cEnd)
    {
        // controllo se il pezzo esiste veramente
        if (!chessBoard[rStart][cStart].isEqual(piece))
            return false;
        // poi eseguo tutti i controlli sulla posizione di arrivo

        chessBoard[rEnd][cEnd] = piece;
        chessBoard[rStart][cStart] = new Piece();

        return true;
    }

    public Piece PieceFromString(String name, String color)
    {
        try {
            Piece.Name nameValue = Piece.Name.valueOf(name.toUpperCase());
            Piece.Color colorValue = Piece.Color.valueOf(color.toUpperCase());
            return new Piece(nameValue, colorValue);
        } catch (IllegalArgumentException e) {
            return null;
        }
    }

    public Posizione calcolaCoordinateOpposte(Posizione posizione) {
        // Assumiamo una scacchiera 8x8
        final int dimensioneScacchiera = 8;

        // Calcola le coordinate opposte riflettendo rispetto al centro della scacchiera
        int rigaOpposta = dimensioneScacchiera - 1 - posizione.riga;
        int colonnaOpposta = dimensioneScacchiera - 1 - posizione.colonna;

        // Aggiorna i valori dentro all'oggetto Posizione
        posizione.riga = rigaOpposta;
        posizione.colonna = colonnaOpposta;

        // Restituisci l'oggetto Posizione modificato
        return posizione;
    }
}
