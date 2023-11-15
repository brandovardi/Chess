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

// split
#include <sstream>
#include <vector>

#include "Chessboard.h"
#include "ClientTCP.h"
#include "StartGame.h"

vector<string> split(const string& s, char delimiter);
bool CheckServerConnection(ClientTCP& server, string colore);

// main code
int main()
{
	UseDoubleBuffering(true); // questa funzione serve per evitare lo "sfarfallio" caricando prima la pagina da disegnare

	StartGame game = StartGame();
	string colore = game.chooseColor();

	Chessboard gameBoard = Chessboard(colore); // creo la scacchiera indicando il colore
	ClientTCP client = ClientTCP();
	bool exit = false; // variabile che controlla il termine del gioco
	bool pressed = false; // variabile per memorizzare se è stato premuto un pezzo
	int mX = -1, mY = -1; // coordinate per memorizzare la posizione del mouse

	if (!CheckServerConnection(client, colore))
		return 0;

	string vincitore = "";
	// genero il suono per l'inizio della partita
	sndPlaySound(L"sounds/game-start.wav", SND_ASYNC);
	// loop per rimanere all'interno del gioco
	bool posiziona = true;
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
			// controllo se è stato effettivamente cliccato un pezzo
			if (gameBoard.PezzoCliccato(mX, mY, true))
			{
				// disegno il pezzo cliccato in movimento mentre tiene premuto il mouse
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
						gameBoard.ControllaMossa(mX, mY);
						posiziona = false;
						break;
					}

					Present();
				}
				// uscito dal ciclo ha lasciato il pezzo con il mouse e quindi lo posiziono
				if (posiziona)
				{
					// richiamo la funzione placePiece() passandogli le coordinate del mouse, per posizionare la pedina
					if (gameBoard.ControllaMossa(MouseX(), MouseY()))
					{
						client.Send(gameBoard.getLastMove());
						string response = client.Recieve();
						vector<string> str = split(response, ';');
						if (str.at(0) == "OK")
						{

						}
					}
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
	// chiudo la connessione con il server (quindi chiudo la socket)
	client.Close();

	// una volta finito il gioco chiudo la finestra
	CloseWindow();

	return 0;
}

bool CheckServerConnection(ClientTCP& server, string colore)
{
	Clear(White);
	if (!server.CreateSocketConnectServer())
	{
		DrawString(Width / 2, 0, "Errore durante la\nconnessione al server", "Century", 20, Black, true);
		Present();
		sndPlaySound(L"sounds/game-end.wav", SND_ASYNC);
		Wait(3000);
		CloseWindow();
		return false;
	}
	// finchè il server non trova un'altro giocatore con un colore diverso dal mio non inizia la partita
	DrawString(Width / 2, 0, "Attendere...", "Century", 20, Black, true);
	Present();
	// invio al server il colore che ho selezionato
	server.Send(colore);
	// attendo quindi la risposta del server per poter iniziare (in attesa di un'altro client)
	while (!server.Recieve()._Equal("start")) {}

	return true;
}

vector<string> split(const string& s, char car) {
	vector<string> splitted = {};
	istringstream startStr(s);
	string element = "";

	while (getline(startStr, element, car))
		splitted.push_back(element);

	return splitted;
}
