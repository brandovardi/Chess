public class Piece {
    // dichiarazione pezzi
    public enum Name {
        KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, NONE
    }
    // dichiarazione colori
    public enum Color {
        WHITE, BLACK, NONE
    }
    // attributes
    private Name name;
    private Color color;
    // default constructor
    public Piece()
    {
        this.name = Name.NONE;
        this.color = Color.NONE;
    }
    // parameters constructor
    public Piece(Name name, Color color)
    {
        this.name = name;
        this.color = color;
    }

    public boolean isEqual(Piece p2)
    {
        return this.name == p2.name && this.color == p2.color;
    }

    // getters
    public Name getName() { return this.name; }
    public Color getColor() { return this.color; }
    // setters
    public void setName(Name name) { this.name = name; }
    public void setColor(Color color) { this.color = color; }
}
