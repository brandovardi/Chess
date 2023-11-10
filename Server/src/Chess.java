public class Chess {
    // costanti
    final public int ROW = 8;
    final public int COLUMN = 8;
    
    // attributi
    private Piece[][] chessBoard;
    private Piece.Color playerColor;
    private Piece.Color oppositeColor;
    private boolean whiteToMove;

    public Chess(String color) {
        this.whiteToMove = true;
        this.chessBoard = new Piece[ROW][COLUMN];

        if (color == null || color.toLowerCase().equals("white"))
        {
            this.playerColor = Piece.Color.WHITE;
            this.oppositeColor = Piece.Color.BLACK;
        }
        else if (color.toLowerCase().equals("black"))
        {
            this.playerColor = Piece.Color.BLACK;
            this.oppositeColor = Piece.Color.WHITE;
        }
        // all pieces
        int i = 0;
        for (Piece.Name piece : Piece.Name.values()) {
            this.chessBoard[0][i] = new Piece(piece, this.oppositeColor);
            this.chessBoard[7][i] = new Piece(piece, this.playerColor);
            i++;
        }
        // all pawns
        for (int j = 0; j < COLUMN; j++) {
            this.chessBoard[1][j] = new Piece(Piece.Name.PAWN, oppositeColor);
            this.chessBoard[6][j] = new Piece(Piece.Name.PAWN, playerColor);
        }
        // riempio tutte le altre celle con dei pezzi vuoti
        for (int j = 2; j < ROW - 2; j++) {
            for (int k = 0; k < COLUMN; k++) {
                this.chessBoard[j][k] = new Piece();
            }
        }
    }

    public boolean CheckPieceMove(Piece piece, int rStart, int cStart, int rEnd, int cEnd)
    {
        // controllo se il pezzo esiste veramente
        if (!chessBoard[rStart][cStart].isEqual(piece))
            return false;
        // poi eseguo tutti i controlli sulla posizione di arrivo

        

        chessBoard[rEnd][cEnd] = chessBoard[rStart][cStart]; // sposto il pezzo
        chessBoard[rStart][cStart] = new Piece(); // e lo rimuovo dalla sua posizione di partenza
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

}
