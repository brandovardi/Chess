#pragma once

#include <thread>

#include "drawing.h"
#include "Piece.h"

const int ROW = 8;
const int COLUMN = 8;
const int CELL_HEIGHT = 57;
const int CELL_WIDTH = 57;
const int X_START_CB = 0;
const int Y_START_CB = 0;
const int MAX_COLORI = 2;
const string imgExt = ".bmp";
const string sndExt = ".wav";
const string SND_PATH = "sounds/";
const Color EatColor = Black;

class Chessboard {
private:
	Piece pezzi[ROW][COLUMN];
	Piece pezzoMosso;
	Piece reSottoScacco;
	bool whiteToMove;
	bool arroccoTmp;
	bool enPassantTmp;
	bool twoFAenPass; // verifica a due fattori per l'en passant :)

	int riga;
	int col;
	int x;
	int y;
	// colori disponibili nel gioco (bianco e nero, che sono riferiti ai nomi delle immagini .bmp)
	string colori[MAX_COLORI];
	
	// variabili per la inviare dati al server
	string messaggio;

	// metodi che mi accorciano il codice per non renderlo troppo lungo (le metto come private perch� non sono
	// delle regole effettive del gioco
	bool verificaPosizioneKingSM(bool condition, int r_, int c_, Piece pezzoMangiato);
	bool posConsentitaSM(Piece& pezzoMangiato);
	void DisegnaMosseDisponibili();
	bool StessoColore(Piece p1, Piece p2);
	void fromXY2RowCol(int _x, int _y, int& _row, int& _col);
	void fromRowCol2XY(int _row, int _col, int& _x, int& _y);

public:
	Chessboard(string firstColor = "white");
	void DisegnaCoordinate();
	void DisegnaPezzi();
	bool PezzoCliccato(int _mX, int _mY);
	void DisegnaInMovimento(int _moveX, int _moveY, bool drag = false);
	bool ControllaPezzi(int _r, int _c, Piece& pMoved);
	void Arrocca(int r1, int c1);
	bool ReSottoScacco(Piece king);
	bool ControllaScacco();
	bool PosizionaPezzo(int _mX, int _mY);
	void CambiaPedina(int _mX, int _mY, bool _mPressed);
	bool ArroccoDxVuoto(Piece pMoved);
	bool ArroccoSxVuoto(Piece pMoved);
	bool ArroccoSenzaScacco(Piece piece, int toC);
	void resetPezzo();
	void playSound(bool promoted, Piece eatenPiece);
	bool ScaccoMatto();
	bool Stallo();
	bool verificaMosseSM(bool stallo = false);
	bool MateFunction(Piece re = Piece());
	bool SpostaPezzoSM(Piece pezzo, int r_, int c_);
	bool performEPforCM(Piece pedina, int r_, int c_, bool parteBassa);
	bool EnPassant(int r, int c, string color);
	string getMessaggio() { return this->messaggio; }
	// questo metodo lo lascio pubblico perch� pu� tornare utile
	string ToLowerStr(string& str);

	~Chessboard() {}
};