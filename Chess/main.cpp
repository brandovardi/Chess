/*
* fonte per la connessione client-server (UDP):
* https://gist.github.com/sunmeat/02b60c8a3eaef3b8a0fb3c249d8686fd
* 
* fonte per la riproduzione dell'audio:
* https://www.youtube.com/watch?v=sbn-7WwXMvo
* 
* libreria drawing.h:
* https://github.com/npiegdon/immediate2d
*/

#include "Chessboard.h"
#include "ClientTCP.h"

string controllaSceltaColore(int mx, int my);
void drawSquare(int mx, int my);
string chooseColor();

// mancano i controlli per lo scacco matto (il controllo per lo scacco normale c'è), lo stallo
int main()
{
	UseDoubleBuffering(true); // questa funzione serve per evitare lo "sfarfallio" caricando prima la pagina da disegnare

	string colore = chooseColor();

	Chessboard gameBoard = Chessboard(colore); // creo la scacchiera indicando il colore
	ClientTCP server = ClientTCP();
	bool exit = false; // variabile che controlla il termine del gioco
	bool pressed = false; // variabile per memorizzare se è stato premuto un pezzo
	int mX = -1, mY = -1; // coordinate per memorizzare la posizione del mouse

	server.initializeSocket();

	string vincitore = "";
	// genero il suono per l'inizio della partita
	sndPlaySound(L"sounds/game-start.wav", SND_ASYNC);
	// loop per rimanere all'interno del gioco
	bool posiziona = false;
	while (!exit)
	{
		// "pulisco" lo schermo
		Clear(White);
		// disegno ogni volta lo sfondo per la scacchiera
		Draw(IMG_PATH + "pattern" + imgExt, 0, 0);
		// poi disegno sia le coordinate che i numeri accanto alla scacchiera
		gameBoard.DisegnaCoordinate();
		gameBoard.DisegnaPezzi();

		// controllo se viene premuto il tasto sinistro
		if (LeftMousePressed())
		{
			mX = MouseX();
			mY = MouseY();
			if (gameBoard.PezzoCliccato(mX, mY))
			{
				while (LeftMousePressed())
				{
					Clear(White);
					// disegno ogni volta lo sfondo per la scacchiera
					Draw(IMG_PATH + "pattern" + imgExt, 0, 0);
					// poi disegno sia le coordinate che i numeri accanto alla scacchiera
					gameBoard.DisegnaCoordinate();
					gameBoard.DisegnaPezzi();

					gameBoard.DisegnaInMovimento(MouseX(), MouseY(), true);

					// questo controllo riposiziona il pezzo in movimento alla sua posizione originale
					// se viene premuto il tasto "esc" e pressed è vera (quindi l'utente sta muovendo un pezzo)
					if (LastKey() == Esc)
					{
						// allora richiamo anche qua la funzione per piazzare il pezzo passandogli come coordinate quelle di partenza
						// (quelle precedentemente salvate)
						gameBoard.PosizionaPezzo(mX, mY);
						posiziona = false;
						break;
					}

					Present();
				}
				if (posiziona)
				{
					// richiamo la funzione placePiece() passandogli le coordinate del mouse, per "piazzare" la pedina
					gameBoard.PosizionaPezzo(MouseX(), MouseY());
				}
				else
					posiziona = true;
			}
		}
		Present();
	}

	sndPlaySound(L"sounds/game-end.wav", SND_ASYNC);

	// "pulisco" lo schermo
	Clear(White);
	if (vincitore._Equal("whiteWin"))
		DrawString(Width / 2, 0, "Vince il Bianco", "Century", 30, Black, true);
	else if (vincitore._Equal("blackWin"))
		DrawString(Width / 2, 0, "Vince il Nero", "Century", 30, Black, true);
	Present();
	Wait(3000);
	// una volta finito il gioco chiudo la finestra
	CloseWindow();

	return 0;
}

string chooseColor()
{
	string colore = "";
	while (true)
	{
		Clear(White);

		DrawString(Width / 2, 0, "Scegli il colore", "Century", 30, Blue, true);

		Draw(IMG_PATH + "white_pawn" + imgExt, (Width / 2) - (CELL_HEIGHT), CELL_HEIGHT);
		Draw(IMG_PATH + "black_pawn" + imgExt, (Width / 2), CELL_HEIGHT);

		drawSquare(MouseX(), MouseY());

		if (LeftMousePressed()) {
			while (LeftMousePressed()) {}
			if (!(colore = controllaSceltaColore(MouseX(), MouseY()))._Equal("")) {
				return colore;
			}
		}

		Present();
	}
}

string controllaSceltaColore(int mx, int my)
{
	if (mx > ((Width / 2) - CELL_WIDTH) && mx < (Width / 2) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		return "white";
	else if (mx > (Width / 2) && mx < ((Width / 2) + CELL_WIDTH) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		return "black";
	return "";
}

void drawSquare(int mx, int my)
{
	if (mx > ((Width / 2) - CELL_WIDTH) && mx < (Width / 2) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		DrawRectangle(((Width / 2) - CELL_WIDTH), CELL_HEIGHT, CELL_WIDTH - 1, CELL_HEIGHT, Black, false);
	else if (mx > (Width / 2) && mx < ((Width / 2) + CELL_WIDTH) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		DrawRectangle((Width / 2), CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, Black, false);
}
