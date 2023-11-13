#include "Chessboard.h"

#include <Windows.h> // utile per riprodurre i suoni

// costruttore di default
Chessboard::Chessboard(string firstColor)
{
	(ToLowerStr(firstColor)._Equal("white") ? colori[1] = "black_" : colori[1] = "white_");
	colori[0] = firstColor.append("_");
	// inserisco tutti i pezzi bianchi
	this->pezzi[ROW - 1][0] = Piece(colori[0] + "rook" + imgExt, ROW - 1, 0);		// torre sinistra
	this->pezzi[ROW - 1][7] = Piece(colori[0] + "rook" + imgExt, ROW - 1, 7);		// torre destra
	this->pezzi[ROW - 1][1] = Piece(colori[0] + "knight" + imgExt, ROW - 1, 1);		// cavallo sinistro
	this->pezzi[ROW - 1][6] = Piece(colori[0] + "knight" + imgExt, ROW - 1, 6);		// cavallo destro
	this->pezzi[ROW - 1][2] = Piece(colori[0] + "bishop" + imgExt, ROW - 1, 2);		// alfiere sinistro
	this->pezzi[ROW - 1][5] = Piece(colori[0] + "bishop" + imgExt, ROW - 1, 5);		// alfiere destro
	// in base alle regole per il posizionamento del Re e della Regina guardo se il bianco sta sopra o sotto
	// e li posiziono in base a questo
	if (colori[0]._Equal("white_")) {
		this->pezzi[ROW - 1][3] = Piece(colori[0] + "queen" + imgExt, ROW - 1, 3);		// Regina
		this->pezzi[ROW - 1][4] = Piece(colori[0] + "king" + imgExt, ROW - 1, 4);		// Re
	}
	else if (colori[1]._Equal("white_")) {
		this->pezzi[ROW - 1][3] = Piece(colori[0] + "king" + imgExt, ROW - 1, 3);		// Re
		this->pezzi[ROW - 1][4] = Piece(colori[0] + "queen" + imgExt, ROW - 1, 4);		// Regina
	}
	for (int i = 0; i < COLUMN; i++)  // pedoni bianchi
		this->pezzi[ROW - 2][i] = Piece(colori[0] + "pawn" + imgExt, ROW - 2, i);
	// inserisco tutti i pezzi neri
	this->pezzi[0][0] = Piece(colori[1] + "rook" + imgExt, 0, 0);		// torre sinistra
	this->pezzi[0][7] = Piece(colori[1] + "rook" + imgExt, 0, 7);		// torre destra
	this->pezzi[0][1] = Piece(colori[1] + "knight" + imgExt, 0, 1);		// cavallo sinistro
	this->pezzi[0][6] = Piece(colori[1] + "knight" + imgExt, 0, 6);		// cavallo destro
	this->pezzi[0][2] = Piece(colori[1] + "bishop" + imgExt, 0, 2);		// alfiere sinistro
	this->pezzi[0][5] = Piece(colori[1] + "bishop" + imgExt, 0, 5);		// alfiere destro
	// in base alle regole per il posizionamento del Re e della Regina guardo se il bianco sta sopra o sotto
	// e li posiziono in base a questo
	if (colori[1]._Equal("black_")) {
		this->pezzi[0][3] = Piece(colori[1] + "queen" + imgExt, 0, 3);		// Regina
		this->pezzi[0][4] = Piece(colori[1] + "king" + imgExt, 0, 4);		// Re
	}
	else if (colori[0]._Equal("black_")) {
		this->pezzi[0][3] = Piece(colori[1] + "king" + imgExt, 0, 3);		// Re
		this->pezzi[0][4] = Piece(colori[1] + "queen" + imgExt, 0, 4);		// Regina
	}
	// tutti i pedoni
	for (int i = 0; i < COLUMN; i++)
	{
		this->pezzi[ROW - 2][i] = Piece(colori[0] + "pawn" + imgExt, ROW - 2, i); // pedoni bianchi
		this->pezzi[1][i] = Piece(colori[1] + "pawn" + imgExt, 1, i); // pedoni neri
	}
	// ora assegno la riga e la colonna a tutte le celle che non hanno il pezzo
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (!pezzi[i][j].Exist()) {
				pezzi[i][j].setRiga(i);
				pezzi[i][j].setCol(j);
			}
		}
	}
	this->pezzoMosso = Piece();
	this->reSottoScacco = Piece();
	this->x = 0;
	this->y = 0;
	this->riga = 0;
	this->col = 0;
	this->whiteToMove = true;
	this->arroccoTmp = false;
	this->twoFAenPass = false;
	this->enPassantTmp = false;
	this->lastMove = "";
}
// modifica la stringa in tolower
string Chessboard::ToLowerStr(string& str)
{
	for (int i = 0; i < str.length(); i++)
		str.at(i) = tolower(str.at(i));
	return str;
}
// disegno le coordinate ai lati della scacchiera
void Chessboard::DisegnaCoordinate()
{
	int n = -8; // default start number
	int num = 0;
	// disegno i numeri
	if (colori[0]._Equal("black_"))
		n = 1;
	for (int i = 0; i < ROW; i++)
	{
		if (i + n < 0)
			num = (i + n) * -1;
		else
			num = i + n;
		DrawString(2, (i * CELL_HEIGHT) + 1, to_string(num), "Century", 9, Black, false);
	}

	char coord = 'A';
	if (colori[0]._Equal("black_"))
		coord = 'H';
	// disegno le lettere
	for (int i = 0; i < COLUMN; i++, coord += (colori[0]._Equal("white_") ? 1 : -1))
		DrawString((i * CELL_WIDTH) + (CELL_WIDTH * 0.9), ((COLUMN - 1) * CELL_HEIGHT) + (CELL_HEIGHT * 0.74),
			string(1, coord), "Century", 8, Black, true);
}
// disegno tutti i pezzi sulla scacchiera
void Chessboard::DisegnaPezzi()
{
	// disegno un controrno nero alla scacchiera
	DrawRectangle(X_START_CB, Y_START_CB, COLUMN * CELL_WIDTH, ROW * CELL_HEIGHT, Black, false);
	// ciclo per disegnare tutti i singoli pezzi sulla scacchiera
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			// prima di disegnare controllo se esiste, altrimenti genererebbe un'eccezione
			if (pezzi[i][j].Exist()) {
				fromRowCol2XY(i, j, x, y);
				Draw(IMG_PATH + pezzi[i][j].Nome(), x, y);
			}
		}
	}
}
// partendo dalle coordinate del mouse salvate dell'utente controllo se è stato selezionato un pezzo
bool Chessboard::PezzoCliccato(int mx, int my)
{
	Piece pieceTmp = Piece(); // istanzio un pezzo tegmporaneo per rendere il codice di seguito più leggibile
	// se esiste già un pezzo in movimento allora esco dal metodo
	if (pezzoMosso.Exist())
		return true;
	// controllo se le coordinate sono all'interno della scacchiera
	if (mx <= CELL_WIDTH * COLUMN && my <= CELL_HEIGHT * ROW && mx >= X_START_CB && my >= Y_START_CB) {
		// vado ad ottenere la riga e la colonna della scacchiera utilizzando le coordinate x e y
		fromXY2RowCol(mx, my, riga, col);
		pieceTmp = pezzi[riga][col]; // mi salvo il pezzo che è stato selezionato per una lettura migliore
		/*
		* questo controllo serve per verificare che il pezzo cliccato esista effettivamente, e controllo anche
		* se al turno del bianco è stato selezionato un pezzo bianco o vicerversa per il nero
		*/
		if (pieceTmp.Exist() && (whiteToMove && pieceTmp.Is("white") || !whiteToMove && pieceTmp.Is("black"))
			/*&& (pieceTmp.Is(colori[0]))*/) {
			// rimuovo il pezzo da dove è stato prezo
			pezzi[riga][col] = Piece(riga, col);
			// assegno allora al "pezzo in movimento" il pezzo cliccato
			pezzoMosso = pieceTmp;
			return true;
		}
	}
	return false;
}
// questo metodo serve per disegnare il pezzo in movimento, seguendo il cursore del mouse
void Chessboard::DisegnaInMovimento(int mx, int my, bool drag)
{
	// con questo thread disegno tutte le possibili mosse che può fare il pezzo in movimento
	thread t(&Chessboard::DisegnaMosseDisponibili, this);
	if (!drag)
	{
		fromRowCol2XY(pezzoMosso.Riga(), pezzoMosso.Col(), x, y);
		Draw(IMG_PATH + pezzoMosso.Nome(), x, y);
		t.join();
		return;
	}
	// disegno il contorno sulla cella dove si trova il mouse
	fromXY2RowCol(mx, my, riga, col);
	fromRowCol2XY(riga, col, x, y);
	{ // disegno il rettangolo bianco
		DrawRectangle(x, y, CELL_WIDTH, CELL_HEIGHT, MakeColor(229, 229, 229), false);
		DrawRectangle(x + 1, y + 1, CELL_WIDTH - 2, CELL_HEIGHT - 2, MakeColor(229, 229, 229), false);
	}
	t.join();
	Draw(IMG_PATH + pezzoMosso.Nome(), mx - CELL_WIDTH / 2, my - CELL_HEIGHT / 2);
}
// controllo che il pezzo possa essere spostato nella posizione desiderata
bool Chessboard::ControllaPezzi(int rDest, int cDest, Piece& pMoved)
{
	// se ha mosso il pezzo su un'altro con lo stesso colore esco subito dal metodo
	if (StessoColore(pezzi[rDest][cDest], pMoved))
		return true;
	// controllo la torre e la regina perché hanno delle mosse simili
	if (pMoved.Is("rook") || pMoved.Is("queen"))
	{
		// se il pezzo non è stato mosso dalla sua posizione originale
		if (rDest == pMoved.Riga() && cDest == pMoved.Col())
			return true;
		// controllo solamente per la torre se è stata spostata in diagonale
		if (pMoved.Is("rook"))
			// semplicemente verificando se sia la colonna che la riga sono diverse
			if (rDest != pMoved.Riga() && cDest != pMoved.Col())
				return true;
		// controllo se la torre si è mossa sulla stessa riga
		if (rDest == pMoved.Riga() && cDest != pMoved.Col())
		{
			// controllo se si è spostata a destra
			if (cDest > pMoved.Col()) {
				// controllo che non ci siano pezzi di qualunque colore sul "percorso" della torre
				for (int i = pMoved.Col() + 1; i < cDest; i++)
					if (pezzi[rDest][i].Exist())
						return true;
			}
			// controllo se si è spostata a sinistra
			else if (cDest < pMoved.Col())
				// controllo che non ci siano pezzi di qualunque colore sul "percorso" della torre
				for (int i = pMoved.Col() - 1; i > cDest; i--)
					if (pezzi[rDest][i].Exist())
						return true;
		}
		// controllo se la torre si è mossa sulla stessa colonna
		else if (cDest == pMoved.Col() && rDest != pMoved.Riga())
		{
			// controllo se si è spostata verso il basso
			if (rDest > pMoved.Riga())
			{
				// controllo che non ci siano pezzi di qualunque colore sul "percorso" della torre
				for (int i = pMoved.Riga() + 1; i < rDest; i++)
					if (pezzi[i][cDest].Exist())
						return true;
			}
			// controllo se si è spostata verso l'alto
			else if (rDest < pMoved.Riga())
				// controllo che non ci siano pezzi di qualunque colore sul "percorso" della torre
				for (int i = pMoved.Riga() - 1; i > rDest; i--)
					if (pezzi[i][cDest].Exist())
						return true;
		}
	}
	// controllo l'alfiere e la regina perché hanno delle mosse simili
	if (pMoved.Is("bishop") || pMoved.Is("queen"))
	{
		// controllo se l'alfiere è stato mosso in verticalo o orizzontale
		if (pMoved.Is("bishop"))
			// basta controllare se la riga o la colonna sono rimaste uguali
			if (rDest == pMoved.Riga() || cDest == pMoved.Col())
				return true;
		// prima di controllare le diagonali verifico che effettivamente si sia spostato in diagonale
		// perché controllare che la riga o che la colonna non siano uguali non basta
		if (pMoved.Riga() != rDest && pMoved.Col() != cDest)
		{
			// quindi prima controllo in che posizione risulta essersi spostato l'alfiere o la regina
			// (controllo se si è spostato in altro a destra o in basso a sinistra)
			if (rDest < pMoved.Riga() && cDest > pMoved.Col() || rDest > pMoved.Riga() && cDest < pMoved.Col())
			{
				// e con un piccolo calcolo controllo che si sia effettivamente mosso sulla diagonale
				// partendo dalla cella di partenza
				if (((rDest - pMoved.Riga()) * -1) != (cDest - pMoved.Col()))
					return true;
			}
			// altrimenti controllo se si è mosso in alto a sinistra o in basso a destra, sempre seguendo la diagonale
			else if (rDest - pMoved.Riga() != cDest - pMoved.Col())
				return true;
		}
		// arrivato qua posso controllare se il "percorso" dell'alfiere o della regina non è ostacolato da nessun pezzo
		if (rDest > pMoved.Riga() && cDest > pMoved.Col())
		{
			// controllo la diagonale che si muove in basso a destra
			for (int r_ = pMoved.Riga() + 1, c_ = pMoved.Col() + 1; r_ < rDest && c_ < cDest; r_++, c_++)
				if (pezzi[r_][c_].Exist())
					return true;
		}
		else if (rDest > pMoved.Riga() && cDest < pMoved.Col())
		{
			// controllo la diagonale che si muove in basso a sinistra
			for (int r_ = pMoved.Riga() + 1, c_ = pMoved.Col() - 1; r_ < rDest && c_ > cDest; r_++, c_--)
				if (pezzi[r_][c_].Exist())
					return true;
		}
		else if (rDest < pMoved.Riga() && cDest < pMoved.Col())
		{
			// controllo la diagonale che si muove in alto a sinistra
			for (int r_ = pMoved.Riga() - 1, c_ = pMoved.Col() - 1; r_ > rDest && c_ > cDest; r_--, c_--)
				if (pezzi[r_][c_].Exist())
					return true;
		}
		else if (rDest < pMoved.Riga() && cDest > pMoved.Col())
			// infine controllo la diagonale che si muove in alto a destra
			for (int r_ = pMoved.Riga() - 1, c_ = pMoved.Col() + 1; r_ > rDest && c_ < cDest; r_--, c_++)
				if (pezzi[r_][c_].Exist())
					return true;
	}
	// controllo il cavallo
	if (pMoved.Is("knight"))
	{
		// controllo se il cavallo ha effettuato una qualsiasi mossa a "L"
		if (!((pMoved.Riga() + 2 == rDest && (pMoved.Col() + 1 == cDest || pMoved.Col() - 1 == cDest))
			|| (pMoved.Riga() - 2 == rDest && (pMoved.Col() - 1 == cDest || pMoved.Col() + 1 == cDest))
			|| (pMoved.Col() + 2 == cDest && (pMoved.Riga() + 1 == rDest || pMoved.Riga() - 1 == rDest))
			|| (pMoved.Col() - 2 == cDest && (pMoved.Riga() - 1 == rDest || pMoved.Riga() + 1 == rDest))))
			return true;
	}
	// controllo il re
	if (pMoved.Is("king"))
	{
		bool _castling = false;
		if (!pMoved.Arrocco())
		{
			// suddivido i controlli andando controllare se si è mosso sulla stessa riga
			// per controllare separatamente l'arrocco
			if (rDest == pMoved.Riga())
			{
				/*
				* Devo sostituire il re sulla scacchiera solo in questo caso (perché potrebbe aver arroccato)
				* perché quando muovo un pezzo lo rimuovo dalla scacchiera e se devo controllare se il re è sotto scacco
				* quando si muove, non posso farlo, ovviamente, perché non posso trovare la sua posizione di partenza.
				* Quindi sostituisco il re per verificare se non è sotto scacco prima di fare l'arrocco."
				*/
				pezzi[pMoved.Riga()][pMoved.Col()] = pMoved;
				if (!ControllaScacco())
				{
					// innanzitutto controllo se è la prima mossa del re (altrimenti non può arroccare)
					// e se sul percorso dell'arrocco non c'è la possibilità di scacco
					if (!pMoved.PrimaMossa() && ArroccoSenzaScacco(pMoved, cDest))
					{
						if (colori[0]._Equal("white_"))
						{
							// controllo se si è mosso verso destra, verificando che anche per la torre sia la prima mossa
							// e che il "percorso" sia libero
							if (cDest == (pMoved.Col() + 2) && pezzi[rDest][cDest + 1].Exist() && !pezzi[rDest][cDest + 1].PrimaMossa()
								&& ArroccoDxVuoto(pMoved))
							{
								// sposto il re
								//pezzi[pMoved.Riga()][pMoved.Col() + 2] = pMoved;
								//pezzi[pMoved.Riga()][pMoved.Col() + 2].setCol(pMoved.Col() + 2);
								//// e libero la sua vecchia cella
								//pezzi[pMoved.Riga()][pMoved.Col() + 2] = Piece(pMoved.Riga(), pMoved.Col() + 2);
								Arrocca(rDest, cDest + 1);
								_castling = true;
							}
							// all'opposto controllo le stesse cose per il lato sinistro
							else if (cDest == (pMoved.Col() - 2) && pezzi[rDest][cDest - 2].Exist() && !pezzi[rDest][cDest - 2].PrimaMossa()
								&& ArroccoSxVuoto(pMoved))
							{
								// sposto il re
								//pezzi[pMoved.Riga()][pMoved.Col() - 2] = pMoved;
								//pezzi[pMoved.Riga()][pMoved.Col() - 2].setCol(pMoved.Col() - 2);
								//// e libero la sua vecchia cella
								//pezzi[pMoved.Riga()][pMoved.Col() - 2] = Piece(pMoved.Riga(), pMoved.Col() - 2);
								Arrocca(rDest, cDest - 2);
								_castling = true;
							}
						}
						else if (colori[0]._Equal("black_"))
						{
							// controllo se si è mosso verso destra, verificando che anche per la torre sia la prima mossa
							// e che il "percorso" sia libero
							if (cDest == (pMoved.Col() + 2) && pezzi[rDest][cDest + 2].Exist() && !pezzi[rDest][cDest + 2].PrimaMossa()
								&& ArroccoDxVuoto(pMoved))
							{
								// sposto il re
								//pezzi[pMoved.Riga()][pMoved.Col() + 2] = pMoved;
								//pezzi[pMoved.Riga()][pMoved.Col() + 2].setCol(pMoved.Col() + 2);
								//// e libero la sua vecchia cella
								//pezzi[pMoved.Riga()][pMoved.Col() + 2] = Piece(pMoved.Riga(), pMoved.Col() + 2);
								Arrocca(rDest, cDest + 2);
								_castling = true;
							}
							// all'opposto controllo le stesse cose per il lato sinistro
							else if (cDest == (pMoved.Col() - 2) && pezzi[rDest][cDest - 1].Exist() && !pezzi[rDest][cDest - 1].PrimaMossa()
								&& ArroccoSxVuoto(pMoved))
							{
								// sposto il re
								//pezzi[pMoved.Riga()][pMoved.Col() - 2] = pMoved;
								//pezzi[pMoved.Riga()][pMoved.Col() - 2].setCol(pMoved.Col() - 2);
								//// e libero la sua vecchia cella
								//pezzi[pMoved.Riga()][pMoved.Col() - 2] = Piece(pMoved.Riga(), pMoved.Col() - 2);
								Arrocca(rDest, cDest - 1);
								_castling = true;
							}
						}
					}
				}
				// dopo aver controllato i vari casi per lo scacco rimuovo nuovamente il re dalla scacchiera
				pezzi[pMoved.Riga()][pMoved.Col()] = Piece(pMoved.Riga(), pMoved.Col());
			}
		}
		// se non ha arroccato allora controllo semplicemente tutte le mosse possibili del re
		if (!_castling)
		{
			if (((pMoved.Riga() + 1 != rDest || pMoved.Col() != cDest)
				&& (pMoved.Riga() + 1 != rDest || pMoved.Col() + 1 != cDest)
				&& (pMoved.Riga() != rDest || pMoved.Col() + 1 != cDest)
				&& (pMoved.Riga() - 1 != rDest || pMoved.Col() + 1 != cDest)
				&& (pMoved.Riga() - 1 != rDest || pMoved.Col() != cDest)
				&& (pMoved.Riga() - 1 != rDest || pMoved.Col() - 1 != cDest)
				&& (pMoved.Riga() != rDest || pMoved.Col() - 1 != cDest)
				&& (pMoved.Riga() + 1 != rDest || pMoved.Col() - 1 != cDest))) {
				return true;
			}
		}
	}
	// controllo i pedoni
	if (pMoved.Is("pawn"))
	{
		bool parteBassa = (pMoved.Is("white") && colori[0]._Equal("white_"))
			|| (pMoved.Is("black") && colori[0]._Equal("black_"));
		bool parteAlta = (pMoved.Is("black") && colori[1]._Equal("black_"))
			|| (pMoved.Is("white") && colori[1]._Equal("white_"));

		// mi salvo la condizione per sapere se è possibile effettuare l'enPassant in questo turno
		bool controlEnPassant = (enPassantTmp && rDest == (pMoved.Riga() + (parteBassa ? -1 : 1))
			&& (cDest == pMoved.Col() - 1 || cDest == pMoved.Col() + 1)
			&& pezzi[rDest + (parteBassa ? 1 : -1)][cDest].Is((parteBassa ? colori[1] : colori[0]) + "pawn")
			&& pezzi[rDest + (parteBassa ? 1 : -1)][cDest].EnPassant());

		// controllo tutte le mosse delle pedine
		if (!((rDest == pMoved.Riga() + (parteBassa ? -1 : 1) && cDest == pMoved.Col() && !pezzi[rDest][cDest].Exist())
			// la pedina può andare avanti di due caselle solamente se è la sua prima mossa e non c'è nessuno
			// sulla casella di destinazione
			|| (rDest == pMoved.Riga() + (parteBassa ? -2 : 2) && cDest == pMoved.Col() && !pezzi[rDest][cDest].Exist()
				&& !pezzi[rDest + (parteBassa ? 1 : -1)][cDest].Exist() && !pMoved.PrimaMossa())
			// la pedina può mangiare solamente in diagonale e solamente i pezzi avversari
			|| (rDest == pMoved.Riga() + (parteBassa ? -1 : 1) && (cDest == pMoved.Col() + 1 || cDest == pMoved.Col() - 1)
				&& pezzi[rDest][cDest].Is((parteBassa ? colori[1] : colori[0])))
			// se possibile la pedina può effettuare l'enPassant
			|| (controlEnPassant))) {
			return true;
		}
		// controllo se la pedina si trova nell'ultimo "rank"
		if ((rDest == 0 && parteBassa) || (rDest == ROW - 1 && parteAlta))
			pMoved.setPromuovi(true);
		// controllo se devo attivare l'enPassant per il turno successivo
		if (rDest == pMoved.Riga() + (parteBassa ? -2 : 2) && cDest == pMoved.Col()
			&& (pezzi[rDest][cDest - 1].Is((parteBassa ? colori[1] : colori[0]) + "pawn")
				|| pezzi[rDest][cDest + 1].Is((parteBassa ? colori[1] : colori[0]) + "pawn")))
		{
			enPassantTmp = true;
			pMoved.setEnPassant(true);
		}
		// controllo se può fare l'en passant
		if (controlEnPassant)
			return EnPassant(rDest, cDest, (parteBassa ? colori[0] : colori[1]));
	}
	return false;
}
// metodo per effettuare l'en passant
bool Chessboard::EnPassant(int r, int c, string color)
{
	// qua implemento le regole per effettuare l'enpassant
	// prima posiziono la pedina bianca nella cella di destinazione
	pezzi[r][c] = pezzoMosso;
	int rTmp = (color._Equal(colori[0])) ? (r + 1) : (r - 1);
	// mi salvo temporaneamente la pedina nera per non doverla ricreare nuovamente dopo
	Piece pawnTmp = pezzi[rTmp][c];
	// poi tolgo la pedina nera dal campo
	pezzi[rTmp][c] = Piece(rTmp, c);
	// prima di continuare controllo che non sia presente uno scacco
	if (ControllaScacco()) {
		// se il re è arrivato sotto scacco allora rimetto tutto a posto come prima
		pezzi[r][c] = Piece(r, c);
		// poi tolgo la pedina nera dal campo
		pezzi[rTmp][c] = pawnTmp;
		enPassantTmp = false;
		twoFAenPass = false;
		// ritorno true perché per risparmiare istruzioni l'ho utilizzata come ritorno per il metodo "ControllaPezzi"
		return true;
	}
	twoFAenPass = false;
	enPassantTmp = false;
	return false;
}
// vado a controllare, secondo le regole  degli scacchi, se il percorso per effettuare l'arrocco non è "affetto da scacco"
bool Chessboard::ArroccoSenzaScacco(Piece king, int destinationCol)
{
	// rimuovo quindi temporaneamente il re dalla sua posizione per poi riposizionarlo sulla cella adiacente
	pezzi[king.Riga()][king.Col()] = Piece(king.Riga(), king.Col());
	// controllo se va verso sinistra
	if (king.Col() > destinationCol)
	{
		// adesso sposto effettivamente il re e controllo se potrebbe essere sotto scacco
		pezzi[king.Riga()][king.Col() - 1] = king;
		pezzi[king.Riga()][king.Col() - 1].setCol(king.Col() - 1);
		if (ReSottoScacco(pezzi[king.Riga()][king.Col() - 1]))
		{
			pezzi[king.Riga()][king.Col() - 1] = Piece(king.Riga(), king.Col() - 1);
			return false;
		}
		// infine lo rimuovo
		pezzi[king.Riga()][king.Col() - 1] = Piece(king.Riga(), king.Col() - 1);
	}
	// o verso destra
	else if (king.Col() < destinationCol)
	{
		// adesso sposto effettivamente il re e controllo se potrebbe essere sotto scacco
		pezzi[king.Riga()][king.Col() + 1] = king;
		pezzi[king.Riga()][king.Col() + 1].setCol(king.Col() + 1);
		if (ReSottoScacco(pezzi[king.Riga()][king.Col() + 1]))
		{
			pezzi[king.Riga()][king.Col() + 1] = Piece(king.Riga(), king.Col() + 1);
			return false;
		}
		// infine lo rimuovo
		pezzi[king.Riga()][king.Col() + 1] = Piece(king.Riga(), king.Col() + 1);
	}
	// e lo riposiziono alla sua posizione originale
	pezzi[king.Riga()][king.Col()] = king;
	return true;
}
// con questo metodo vado ad effettuare l'arrocco sulla scacchiera
void Chessboard::Arrocca(int rT, int cT) // rigaTorre, colonnaTorre
{
	// controllo se sta facendo l'arrocco lungo
	if (pezzoMosso.Col() > pezzi[rT][cT].Col())
	{
		if (colori[0]._Equal("white_"))
		{
			// muovo la torre dove dovrebbe andare dopo l'arrocco
			pezzi[rT][cT + 3] = Piece(pezzi[rT][cT].Nome(), pezzi[rT][cT].Riga(), pezzi[rT][cT].Col() + 3, true, true);
			// e la rimuovo da dove l'ho spostata
			pezzi[rT][cT] = Piece(rT, cT);
		}
		else if (colori[0]._Equal("black_"))
		{
			// muovo la torre dove dovrebbe andare dopo l'arrocco
			pezzi[rT][cT + 2] = Piece(pezzi[rT][cT].Nome(), pezzi[rT][cT].Riga(), pezzi[rT][cT].Col() + 2, true, true);
			// e la rimuovo da dove l'ho spostata
			pezzi[rT][cT] = Piece(rT, cT);
		}
	}
	// o l'arrocco corto
	else if (pezzoMosso.Col() < pezzi[rT][cT].Col())
	{
		if (colori[0]._Equal("white_"))
		{
			// muovo la torre dove dovrebbe andare dopo l'arrocco
			pezzi[rT][cT - 2] = Piece(pezzi[rT][cT].Nome(), pezzi[rT][cT].Riga(), pezzi[rT][cT].Col() - 2, true, true);
			// e la rimuovo da dove l'ho spostata
			pezzi[rT][cT] = Piece(rT, cT);
		}
		else if (colori[0]._Equal("black_"))
		{
			// muovo la torre dove dovrebbe andare dopo l'arrocco
			pezzi[rT][cT - 3] = Piece(pezzi[rT][cT].Nome(), pezzi[rT][cT].Riga(), pezzi[rT][cT].Col() - 3, true, true);
			// e la rimuovo da dove l'ho spostata
			pezzi[rT][cT] = Piece(rT, cT);
		}
	}
	pezzoMosso.setArrocco(true);
	arroccoTmp = true;
}
// questo metodo controlla se il re è sotto scacco nella posizione dove è stato posizionato dal giocatore
bool Chessboard::ReSottoScacco(Piece king)
{
	Piece piece = Piece();
	// controllo se potrebbe essere mangiato da una torre o da una regina
	for (int i = king.Riga() + 1; i < ROW; i++) // in basso
	{
		piece = pezzi[i][king.Col()];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("rook") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}
	for (int i = king.Riga() - 1; i >= 0; i--) // in alto
	{
		piece = pezzi[i][king.Col()];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("rook") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}
	for (int i = king.Col() + 1; i < COLUMN; i++) // a destra
	{
		piece = pezzi[king.Riga()][i];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("rook") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}
	for (int i = king.Col() - 1; i >= 0; i--) // a sinistra
	{
		piece = pezzi[king.Riga()][i];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("rook") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}

	// controllo se potrebbe essere mangiato da un'alfiere o da una regina
	// controllo la diagonale in basso a destra
	for (int rK = king.Riga() + 1, cK = king.Col() + 1; rK < ROW && cK < COLUMN; rK++, cK++)
	{
		piece = pezzi[rK][cK];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("bishop") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}
	// controllo la diagonale in basso a sinistra
	for (int rK = king.Riga() + 1, cK = king.Col() - 1; rK < ROW && cK >= 0; rK++, cK--)
	{
		piece = pezzi[rK][cK];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("bishop") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}
	// controllo la diagonale in alto a destra
	for (int rK = king.Riga() - 1, cK = king.Col() + 1; rK >= 0 && cK < COLUMN; rK--, cK++)
	{
		piece = pezzi[rK][cK];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("bishop") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}
	// controllo la diagonale in alto a sinistra
	for (int rK = king.Riga() - 1, cK = king.Col() - 1; rK >= 0 && cK >= 0; rK--, cK--)
	{
		piece = pezzi[rK][cK];
		if (piece.Exist())
		{
			if (!StessoColore(piece, king) && (piece.Is("bishop") || piece.Is("queen")))
				return true;
			else
				break;
		}
	}

	// controllo se potrebbe essere mangiato da un cavallo
	if (king.Riga() < ROW - 2 && king.Col() < COLUMN - 1)
	{
		piece = pezzi[king.Riga() + 2][king.Col() + 1];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() < ROW - 2 && king.Col() >= 1)
	{
		piece = pezzi[king.Riga() + 2][king.Col() - 1];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() >= 2 && king.Col() >= 1)
	{
		piece = pezzi[king.Riga() - 2][king.Col() - 1];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() >= 2 && king.Col() < COLUMN - 1)
	{
		piece = pezzi[king.Riga() - 2][king.Col() + 1];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() < ROW - 1 && king.Col() < COLUMN - 2)
	{
		piece = pezzi[king.Riga() + 1][king.Col() + 2];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() < ROW - 1 && king.Col() >= 2)
	{
		piece = pezzi[king.Riga() + 1][king.Col() - 2];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() >= 1 && king.Col() >= 2)
	{
		piece = pezzi[king.Riga() - 1][king.Col() - 2];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}
	if (king.Riga() >= 1 && king.Col() < COLUMN - 2)
	{
		piece = pezzi[king.Riga() - 1][king.Col() + 2];
		if (piece.Exist() && !StessoColore(piece, king) && piece.Is("knight"))
			return true;
	}

	// controllo se potrebbe essere mangiato da un pedone
	if ((king.Is("white") && colori[0]._Equal("white_")) || (king.Is("black") && colori[0]._Equal("black_")))
	{
		if (king.Riga() >= 1 && king.Col() < COLUMN - 1)
		{
			piece = pezzi[king.Riga() - 1][king.Col() + 1];
			if (piece.Exist() && !StessoColore(piece, king) && piece.Is("pawn"))
				return true;
		}
		if (king.Riga() >= 1 && king.Col() >= 1)
		{
			piece = pezzi[king.Riga() - 1][king.Col() - 1];
			if (piece.Exist() && !StessoColore(piece, king) && piece.Is("pawn"))
				return true;
		}
	}
	else if ((king.Is("black") && colori[1]._Equal("black_")) || (king.Is("white") && colori[1]._Equal("white_")))
	{
		if (king.Riga() < ROW - 1 && king.Col() < COLUMN - 1)
		{
			piece = pezzi[king.Riga() + 1][king.Col() + 1];
			if (piece.Exist() && !StessoColore(piece, king) && piece.Is("pawn"))
				return true;
		}
		if (king.Riga() < ROW - 1 && king.Col() >= 1)
		{
			piece = pezzi[king.Riga() + 1][king.Col() - 1];
			if (piece.Exist() && !StessoColore(piece, king) && piece.Is("pawn"))
				return true;
		}
	}

	// controllo se potrebbe essere mangiato dall'altro re
	if (king.Riga() >= 1 && king.Col() >= 1)
	{
		piece = pezzi[king.Riga() - 1][king.Col() - 1];	// in alto a sinistra
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Riga() >= 1)
	{
		piece = pezzi[king.Riga() - 1][king.Col()]; // in alto in mezzo
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Riga() >= 1 && king.Col() < COLUMN - 1)
	{
		piece = pezzi[king.Riga() - 1][king.Col() + 1]; // in alto a destra
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Col() < COLUMN - 1)
	{
		piece = pezzi[king.Riga()][king.Col() + 1]; // a destra in mezzo
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Col() < COLUMN - 1 && king.Riga() < ROW - 1)
	{
		piece = pezzi[king.Riga() + 1][king.Col() + 1]; // in basso a destra
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Riga() < ROW - 1)
	{
		piece = pezzi[king.Riga() + 1][king.Col()]; // in basso al centro
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Riga() < ROW - 1 && king.Col() >= 1)
	{
		piece = pezzi[king.Riga() + 1][king.Col() - 1]; // in basso a sinistra
		if (piece.Exist() && piece.Is("king"))
			return true;
	}
	if (king.Col() >= 1)
	{
		piece = pezzi[king.Riga()][king.Col() - 1]; // in mezzo a sinistra
		if (piece.Exist() && piece.Is("king"))
			return true;
	}

	return false;
}
// questo metodo controlla se sulla scacchiera è presente uno scacco
bool Chessboard::ControllaScacco()
{
	// vado quindi a cercare i due re e controllare se uno dei due è sotto scacco
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (pezzi[i][j].Exist() && pezzi[i][j].Is("king"))
			{
				// controllo
				if (ReSottoScacco(pezzi[i][j])) {
					if (pezzoMosso.Nome() == pezzi[i][j].Nome())
					{
						reSottoScacco = pezzi[i][j];
						return true;
					}
					else if (pezzoMosso.Is("king") && !StessoColore(pezzoMosso, pezzi[i][j]))
						continue;
					reSottoScacco = pezzi[i][j];
					return true;
				}
			}
		}
	}
	return false;
}
// controllo se c'è uno scacco matto (quando il re è sotto scacco e non si può salvare in nessun modo)
bool Chessboard::ScaccoMatto()
{
	return ControllaScacco() && verificaMosseSM();
}
// verifico se è presente una posizione di stallo
bool Chessboard::Stallo()
{
	return !ControllaScacco() && verificaMosseSM(true);
}
// controllo tutte le mosse per verificare la presenta di uno scacco matto o uno stallo
bool Chessboard::verificaMosseSM(bool stallo)
{
	if (!stallo)
		return MateFunction();
	// se sto controllando lo stallo allora cerco i re da verificare
	for (int _r = 0; _r < ROW; _r++)
	{
		for (int _c = 0; _c < COLUMN; _c++)
		{
			if (pezzi[_r][_c].Exist() && pezzi[_r][_c].Is("king"))
			{
				// ritorno true se trovo una posizione di stallo altrimenti vado avanti controllanto l'altro re
				if (MateFunction(pezzi[_r][_c]))
					return true;
			}
		}
	}
	// se arrivo qua significa che non c'è ne scacco matto ne uno stallo quindi resetto il reSottoScacco
	reSottoScacco = Piece();
	return false;
}
// non sapevo che nome dargli ma l'ho creata per il semplice fatto che se devo controllare lo scacco matto
// è più semplice perché prima controllo se c'è lo scacco e se è vero allora mi ritorna un re sotto scacco
// mentre per lo stallo non ci deve essere un re sotto scacco quindi il re sotto scacco è nullo
// e devo cercarlo per verificare lo stallo, il problema è che devo controllare tutti e due i re per vedere se
// c'è nè uno in posizione di stallo
bool Chessboard::MateFunction(Piece re)
{
	if (re.Exist() && !reSottoScacco.Exist())
		reSottoScacco = re;
	Piece pezzoMangiato = Piece();
	int nCondition = 8;
	bool conditions[] = {
		(reSottoScacco.Riga() >= 1 && reSottoScacco.Col() >= 1), // in alto a sinistra
		(reSottoScacco.Riga() >= 1), // in alto al centro
		(reSottoScacco.Riga() >= 1 && reSottoScacco.Col() < COLUMN - 1), // in alto a destra
		(reSottoScacco.Col() < COLUMN - 1), // in mezzo a destra
		(reSottoScacco.Col() < COLUMN - 1 && reSottoScacco.Riga() < ROW - 1), // in basso a destra
		(reSottoScacco.Riga() < ROW - 1), // in basso al centro
		(reSottoScacco.Riga() < ROW - 1 && reSottoScacco.Col() >= 1), // in basso a sinistra
		(reSottoScacco.Col() >= 1), // in mezzo a sinistra
	};
	int positions[][2] = {
		{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }
	};
	// successivamente controllo se il re si può salvare in qualche modo muovendosi
	// o mangiando eventualmente un pezzo
	for (int i = 0; i < nCondition; i++)
	{
		if (!verificaPosizioneKingSM(conditions[i], reSottoScacco.Riga() + positions[i][0],
			reSottoScacco.Col() + positions[i][1], pezzoMangiato))
			return false;
	}
	// poi controllose c'è un pezzo che può "salvare" il re dallo scacco
	// sfortunatamente mi serve ciclare tutta la matrice, poi cercare tutti i pezzi dello stesso colore del
	// re sotto scacco e controllare se quel pezzo spostandolo può salvare il re dallo scacco :)
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			// quindi ora controllo se il pezzo corrente è dello stesso colore del re sotto scacco
			if (StessoColore(reSottoScacco, pezzi[i][j]))
			{
				// controllo le possibili posizioni delle torri (e anche la regina)
				if (pezzi[i][j].Is("rook") || pezzi[i][j].Is("queen"))
				{
					// controllo tutte le posizioni verso destra
					if (j < COLUMN - 1)
					{
						for (int k = j + 1; k < COLUMN; k++)
						{
							// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[i][k], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], i, k))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[i][k], pezzi[i][j]))
								break;
						}
					}
					// controllo tutte le posizioni verso sinistra
					if (j >= 1)
					{
						for (int k = j - 1; k >= 0; k--)
						{
							// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[i][k], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], i, k))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[i][k], pezzi[i][j]))
								break;
						}
					}
					// controllo tutte le posizioni verso il basso
					if (i < ROW - 1)
					{
						for (int k = i + 1; k < ROW; k++)
						{
							// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[k][j], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], k, j))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[k][j], pezzi[i][j]))
								break;
						}
					}
					// controllo tutte le posizioni verso il l'alto
					if (i >= 1)
					{
						for (int k = i - 1; k >= 0; k--)
						{
							// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[k][j], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], k, j))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[k][j], pezzi[i][j]))
								break;
						}
					}
				}
				// controllo l'alfiere e la regina
				if (pezzi[i][j].Is("bishop") || pezzi[i][j].Is("queen"))
				{
					// controllo la diagonale che si muove in basso a destra
					if (i < ROW - 1 && j < COLUMN - 1)
					{
						for (int _r = i + 1, _c = j + 1; _r < ROW && _c < COLUMN; _r++, _c++)
						{
							// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], _r, _c))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
						}
					}
					// controllo la diagonale che si muove in basso a sinistra
					if (i < ROW - 1 && j >= 1)
					{
						for (int _r = i + 1, _c = j - 1; _r < ROW && _c >= 0; _r++, _c--)
						{								// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], _r, _c))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
						}
					}
					// controllo la diagonale che si muove in alto a sinistra
					if (i >= 1 && j >= 1)
					{
						for (int _r = i - 1, _c = j - 1; _r >= 0 && _c >= 0; _r--, _c--)
						{								// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], _r, _c))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
						}
					}
					// infine controllo la diagonale che si muove in alto a destra
					if (i >= 1 && j < COLUMN - 1)
					{
						for (int _r = i - 1, _c = j + 1; _r >= 0 && _c < COLUMN; _r--, _c++)
						{								// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
							if (StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
							else
							{
								// ritorna true se spostando il pezzo non c'è più scacco
								if (SpostaPezzoSM(pezzi[i][j], _r, _c))
									return false;
							}
							// dopo aver controllato la posizione per il pezzo guardo se ho incontrato un pezzo
							// del colore opposto, quindi prcedo con i controlli
							if (!StessoColore(pezzi[_r][_c], pezzi[i][j]))
								break;
						}
					}
				}
				// controllo il cavallo
				else if (pezzi[i][j].Is("knight"))
				{
					// "L" verso il basso a destra
					if (i < ROW - 2 && j < COLUMN - 1)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i + 2][j + 1], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + 2), (j + 1)))
								return false;
					}
					// "L" verso il basso a sinistra
					if (i < ROW - 2 && j >= 1)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i + 2][j - 1], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + 2), (j - 1)))
								return false;
					}
					// "L" verso l'alto a sinistra
					if (i >= 2 && j >= 1)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i - 2][j - 1], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i - 2), (j - 1)))
								return false;
					}
					// "L" verso l'alto a destra
					if (i >= 2 && j < COLUMN - 1)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i - 2][j + 1], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i - 2), (j + 1)))
								return false;
					}
					// "L" verso destra in basso
					if (i < ROW - 1 && j < COLUMN - 2)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i + 1][j + 2], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + 1), (j + 2)))
								return false;
					}
					// "L" verso sinistra in basso
					if (i < ROW - 1 && j >= 2)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i + 1][j - 2], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + 1), (j - 2)))
								return false;
					}
					// "L" verso sinistra in alto
					if (i >= 1 && j >= 2)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i - 1][j - 2], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i - 1), (j - 2)))
								return false;
					}
					// "L" verso destra in alto
					if (i >= 1 && j < COLUMN - 2)
					{
						// se trovo un pezzo dello stesso colore allora esco e procedo con i controlli
						if (!StessoColore(pezzi[i - 1][j + 2], pezzi[i][j]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i - 1), (j + 2)))
								return false;
					}
				}
				// controllo le pedine
				else if (pezzi[i][j].Is("pawn"))
				{
					bool parteBassa = (colori[0]._Equal("white_") && pezzi[i][j].Is("white"))
						|| (colori[0]._Equal("black_") && pezzi[i][j].Is("black"));
					bool parteAlta = (colori[1]._Equal("white_") && pezzi[i][j].Is("white"))
						|| (colori[1]._Equal("black_") && pezzi[i][j].Is("black"));

					// muovo la pedina in avanti di due
					if (!pezzi[i][j].PrimaMossa() && ((parteBassa && i == ROW - 2) || (parteAlta && i == 1)))
					{
						// se c'è un pezzo non posso muovere la pedina
						if (!pezzi[i + (parteBassa ? -2 : 2)][j].Exist())
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + (parteBassa ? -2 : 2)), j))
								return false;
					}
					// controllo che si possa spostare avanti ancora di almeno una casella
					if ((parteBassa && i >= 1) || (parteAlta && i < ROW - 1))
					{
						// se c'è un pezzo non posso muovere la pedina
						if (!pezzi[i + (parteBassa ? -1 : 1)][j].Exist())
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + (parteBassa ? -1 : 1)), j))
								return false;
					}
					// controllo se può eventualmente mangiare a destra
					if ((parteBassa && i >= 1 && j < COLUMN - 1) || (parteBassa && i < ROW - 1 && j < COLUMN - 1))
					{
						// se c'è una pedina del colore opposto allora posso mangiare
						if (pezzi[i + (parteBassa ? -1 : 1)][j + 1].Exist()
							&& !StessoColore(pezzi[i][j], pezzi[i + (parteBassa ? -1 : 1)][j + 1]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + (parteBassa ? -1 : 1)), (j + 1)))
								return false;
					}
					// controllo se può eventualmente mangiare a sinistra
					if ((parteBassa && i >= 1 && j >= 1) || (parteAlta && i < ROW - 1 && j >= 1))
					{
						// se c'è una pedina del colore opposto allora posso mangiare
						if (pezzi[i + (parteBassa ? -1 : 1)][j - 1].Exist()
							&& !StessoColore(pezzi[i][j], pezzi[i + (parteBassa ? -1 : 1)][j - 1]))
							// ritorna true se spostando il pezzo non c'è più scacco
							if (SpostaPezzoSM(pezzi[i][j], (i + (parteBassa ? -1 : 1)), (j - 1)))
								return false;
					}
					if ((parteBassa && i == 3 && (j < COLUMN - 1 || j >= 1))
						|| (parteAlta && i == 4 && (j < COLUMN - 1 || j >= 1)))
					{
						// controllo le condizioni necessarie per l'En Passant
						if (twoFAenPass && (pezzi[i][j + 1].Is("pawn") && !StessoColore(pezzi[i][j], pezzi[i][j + 1])
							&& pezzi[i][j + 1].EnPassant()
							|| (pezzi[i][j - 1].Is("pawn") && !StessoColore(pezzi[i][j], pezzi[i][j - 1])
								&& pezzi[i][j - 1].EnPassant())))
						{
							if (performEPforCM(pezzi[i][j], i, j, parteBassa))
								return false;
						}
					}
				}
			}
		}
	}
	if (re.Exist())
		reSottoScacco = Piece();
	// se arriva alla fine allora è scacco matto (o stallo, dipende dal metodo chiamante)
	return true;
}
// metodo per "simulare" l'en passant per vedere se potrebbe rimuovere lo scacco dalla scacchiera
// e quindi evitare un possibile scacco matto o eventuale stallo
// ritorna true se è possibile effettuare l'en passant
bool Chessboard::performEPforCM(Piece pedina, int r_, int c_, bool parteBassa)
{
	bool destra = pezzi[r_][c_ + 1].EnPassant();
	Piece opponentPawn = pezzi[r_][c_ + (destra ? 1 : -1)];
	Piece pawn = pedina;
	// sposto il pezzo
	pezzi[r_ + (parteBassa ? -1 : 1)][c_ + (destra ? 1 : -1)] = pedina;
	// aggiorno riga e colonna dopo averlo spostato
	pezzi[r_ + (parteBassa ? -1 : 1)][c_ + (destra ? 1 : -1)].setRiga(r_ + (parteBassa ? -1 : 1));
	pezzi[r_ + (parteBassa ? -1 : 1)][c_ + (destra ? 1 : -1)].setCol(c_ + (destra ? 1 : -1));
	// infine rimuovo la pedina che dovrei mangiare facendo l'en passant
	pezzi[r_][c_ + (destra ? 1 : -1)] = Piece(r_, c_ + (destra ? 1 : -1));
	if (!ControllaScacco())
	{
		pezzi[r_ + (parteBassa ? -1 : 1)][c_ + (destra ? 1 : -1)] = Piece(r_ + (parteBassa ? -1 : 1), c_ + (destra ? 1 : -1));
		pezzi[r_][c_ + (destra ? 1 : -1)] = opponentPawn;
		pezzi[pawn.Riga()][pawn.Col()] = pawn;
		return true;
	}
	pezzi[r_ + (parteBassa ? -1 : 1)][c_ + (destra ? 1 : -1)] = Piece(r_ + (parteBassa ? -1 : 1), c_ + (destra ? 1 : -1));
	pezzi[r_][c_ + (destra ? 1 : -1)] = opponentPawn;
	pezzi[pawn.Riga()][pawn.Col()] = pawn;
	return false;
}
// questo metodo sposta un pezzo nella posizione desiderata verificando se è possibile, controllando lo scacco
bool Chessboard::SpostaPezzoSM(Piece pezzo, int r_, int c_)
{
	Piece pezzoMangiato = pezzi[r_][c_]; // mi salvo il pezzo eventualmente mangiato
	Piece pieceTmp = pezzo;
	pezzi[r_][c_] = pezzo; // sposto il pezzo nella posizione indicata
	// modifico la riga e la colonna siccome ho mosso il pezzo in un'altra casella
	pezzi[r_][c_].setRiga(pezzoMangiato.Riga());
	pezzi[r_][c_].setCol(pezzoMangiato.Col());
	pezzi[pezzo.Riga()][pezzo.Col()] = Piece(pezzo.Riga(), pezzo.Col()); // e lo rimuovo dalla pos. originale
	// se non c'è più scacco allora ritorno true
	if (!ControllaScacco())
	{
		pezzi[r_][c_] = pezzoMangiato; // rimetto il pezzo eventualmente mangiato al suo posto originale
		pezzi[pezzo.Riga()][pezzo.Col()] = pieceTmp;
		return true;
	}
	pezzi[r_][c_] = pezzoMangiato; // rimetto il pezzo eventualmente mangiato al suo posto originale
	pezzi[pezzo.Riga()][pezzo.Col()] = pieceTmp;
	return false;
}
// questo metodo controlla se il re si può mouvere in una determinata direzione, se può provo a spostarlo
// e vedere se c'è lo scacco
bool Chessboard::verificaPosizioneKingSM(bool condition, int r_, int c_, Piece pezzoMangiato)
{
	if (condition)
	{
		pezzoMangiato = pezzi[r_][c_];
		if (posConsentitaSM(pezzoMangiato))
			return false;
	}
	return true;
}
// questo metodo sposta i pezzi per verificare se c'è una posizione che possa liberare il re dallo scacco
// la variabile pezzoMangiato mi serve sia per salvarmi l'eventuale pezzo mangiato dal pezzo mosso
// me mi è utile anche per avere appunto la posizione di dove sto spostando il pezzo
bool Chessboard::posConsentitaSM(Piece& pezzoMangiato)
{
	if (StessoColore(pezzoMangiato, reSottoScacco))
		return false;
	Piece tmpMangiato = pezzoMangiato;
	Piece reTmp = reSottoScacco;
	pezzi[tmpMangiato.Riga()][tmpMangiato.Col()] = reTmp;
	// modifico la riga e la colonna del re "virtualmente" (non sapevo che termine usare)
	pezzi[tmpMangiato.Riga()][tmpMangiato.Col()].setCol(tmpMangiato.Col());
	pezzi[tmpMangiato.Riga()][tmpMangiato.Col()].setRiga(tmpMangiato.Riga());
	// infine rimuovo remporaneamente il re dalla sua posizione originale
	pezzi[reTmp.Riga()][reTmp.Col()] = Piece(reTmp.Riga(), reTmp.Col());
	// se non è sotto scacco allora significa che la mossa si può fare e non è scacco matto
	if (!ControllaScacco())
	{
		// riassegno il reSottoScacco perché nel metodo del ControlloScacco vado a salvarmi il re che è sotto scacco
		// e siccome ora l'ho spostato il re se mi ritorna vero allora mi ha salvato il Re che però non è nella sua posizione
		// esatta
		reSottoScacco = reTmp;
		// prima rimetto tutto a posto
		pezzi[tmpMangiato.Riga()][tmpMangiato.Col()] = tmpMangiato;
		pezzi[reTmp.Riga()][reTmp.Col()] = reTmp;
		return true;
	}
	// rimetto tutto a posto come prima dopo aver controllato lo scacco
	reSottoScacco = reTmp;
	pezzi[tmpMangiato.Riga()][tmpMangiato.Col()] = tmpMangiato;
	pezzi[reTmp.Riga()][reTmp.Col()] = reTmp;
	return false;
}
// questo è il metodo principale per poter controllare se il pezzo mosso può essere posizionato
// controllando tutti i casi possibili
bool Chessboard::PosizionaPezzo(int mx, int my)
{
	// prima controllo se il pezzo esiste e se le coordinate passate sono all'interno della scacchiera
	if (pezzoMosso.Exist() && mx <= COLUMN * CELL_WIDTH && my <= ROW * CELL_HEIGHT
		&& mx >= X_START_CB && my >= Y_START_CB)
	{
		// poi vado a ricavare la riga e la colonna
		fromXY2RowCol(mx, my, riga, col);
		// controllo prima che non ci sia nessuno scacco presente, poi verifico che se c'è uno scacco
		// ma il pezzo mosso è dello stesso colore del re sotto scacco allora controllo se la mossa del pezzo
		// potrebbe salvare il re
		// infine controllo se la mossa effettuata è valida
		if ((!ControllaScacco() || (ControllaScacco() && StessoColore(reSottoScacco, pezzoMosso)))
			&& !ControllaPezzi(riga, col, pezzoMosso))
		{
			// vado a salvarmi il pezzo che è stato eventualmente mangiato
			Piece pezzoMangiato = pezzi[riga][col];
			// questa variabile mi serve solamente per riprodurre il suono corretto nel caso di una promozione
			bool promosso = false;
			// una volta superato il controllo vado a posizionare il pezzo mosso dove dovrebbe andare
			// (anche per poter effettuare gli ultimi controlli)
			pezzi[riga][col] = Piece(pezzoMosso.Nome(), riga, col, pezzoMosso.PrimaMossa(), pezzoMosso.Arrocco(), 
				pezzoMosso.Promuovi(), pezzoMosso.EnPassant());
			// vado a controllare se il pezzo deve essere promosso, quindi è una pedina
			if (pezzoMosso.Promuovi())
			{
				promosso = true;
				// metto un ciclo vuoto per dare il tempo all'utente di lasciare il tasto del mouse prima di scegliere
				while (LeftMousePressed()) {}
				// ora rimango dentro il ciclo per far scegliere il nuovo pezzo al giocatore
				while (pezzoMosso.Promuovi())
					CambiaPedina(MouseX(), MouseY(), LeftMousePressed());
			}
			// ora vado a controllare se il re è sotto scacco
			if (ControllaScacco() && StessoColore(pezzoMosso, reSottoScacco))
			{
				// quindi controllo se il re ha già arroccato e con la variabile arroccoTmp verifico che
				// abbia arroccato in questo turno
				if (pezzoMosso.Is("king") && pezzoMosso.Arrocco() && arroccoTmp)
				{
					// rimetto aposto l'arrocco
					bool bianco = colori[0]._Equal("white_");
					// controllo se ha arroccato corto
					if (col > pezzoMosso.Col())
					{
						// rimetto il re al suo posto
						pezzi[pezzoMosso.Riga()][pezzoMosso.Col()] = pezzi[riga][col];
						// e tolgo da dove l'avevo messo
						pezzi[riga][col] = Piece(riga, col);
						// poi rimetto la torre al suo posto
						pezzi[riga][col + (bianco ? 1 : 2)] = pezzi[riga][col - 1];
						pezzi[riga][col + (bianco ? 1 : 2)].setPrimaMossa(false);
						pezzi[riga][col + (bianco ? 1 : 2)].setRiga(riga);
						pezzi[riga][col + (bianco ? 1 : 2)].setCol(col + (bianco ? 1 : 2));
						pezzi[riga][col - 1] = Piece(riga, col - 1);
					}
					// o se ha arroccato lungo
					else if (col < pezzoMosso.Col())
					{
						// rimetto il re al suo posto
						pezzi[pezzoMosso.Riga()][pezzoMosso.Col()] = pezzi[riga][col];
						// e tolgo da dove l'avevo messo
						pezzi[riga][col] = Piece(riga, col);
						// poi rimetto la torre al suo posto
						pezzi[riga][col + (bianco ? -2 : -1)] = pezzi[riga][col + 1];
						pezzi[riga][col + (bianco ? -2 : -1)].setPrimaMossa(false);
						pezzi[riga][col + (bianco ? -2 : -1)].setRiga(riga);
						pezzi[riga][col + (bianco ? -2 : -1)].setCol(col + (bianco ? -2 : -1));
						pezzi[riga][col + 1] = Piece(riga, col + 1);
					}
					pezzoMangiato = Piece();
				}
				else {
					// questo else serve esculsivamente per rimettere a posto la cella sulla quale un pezzo ha provato
					// a muoversi ma spostandosi avrebbe messo il proprio re sotto scacco quindi siccome prima ho dovuto
					// piazzare il pezzo mosso nella posizione desiderata ora devo rimetterlo a posto sistemando tutto
					pezzi[riga][col] = pezzoMangiato;
				}
				pezzoMosso.setArrocco(false);
				//pezzoMosso.setPrimaMossa(false);
				resetPezzo();
				return false;
			}
			// una volta superati tutti i controlli con successo indico che il pezzo ha effettuato la sua prima mossa
			pezzi[riga][col].setPrimaMossa(true);
			if (twoFAenPass) {
				enPassantTmp = false;
				twoFAenPass = false;
			}
			if (enPassantTmp && !twoFAenPass)
				twoFAenPass = true;

			// salvo il pezzo mosso in una stringa
			lastMove = pezzi[riga][col].getServerString() + ";" + to_string(pezzoMosso.Riga()) + ";"
				+ to_string(pezzoMosso.Col()) + "\t";

			if (!pezzoMosso.Is("king") && ScaccoMatto())
				lastMove.append("ScaccoMatto");
			else if (!pezzoMosso.Is("king") && Stallo())
				lastMove.append("Stallo");

			// genero i suoni per la mossa effettuata
			playSound(promosso, pezzoMangiato);
			pezzoMosso = Piece();
			whiteToMove = !whiteToMove;
			reSottoScacco = Piece();
			arroccoTmp = false;
			return true;
		}
	}
	resetPezzo();
	return false;
}
// metodo per generare il suono in base alla mossa effettuata dal giocatore
void Chessboard::playSound(bool promoted, Piece eatenPiece)
{
	if (arroccoTmp)
		sndPlaySound(L"sounds/castle.wav", SND_ASYNC);
	else if (ControllaScacco())
	{
		if (!ScaccoMatto())
			sndPlaySound(L"sounds/move-check.wav", SND_ASYNC);
		else
			sndPlaySound(L"sounds/capture.wav", SND_ASYNC);
	}
	else if (promoted)
		sndPlaySound(L"sounds/promote.wav", SND_ASYNC);
	else if (eatenPiece.Exist())
		sndPlaySound(L"sounds/capture.wav", SND_ASYNC);
	else
	{
		if (pezzoMosso.Is(colori[0]))
			sndPlaySound(L"sounds/move-self.wav", SND_ASYNC);
		else if (pezzoMosso.Is(colori[1]))
			sndPlaySound(L"sounds/move-opponent.wav", SND_ASYNC);
	}
}
// resetto il pezzo appena mosso alla sua posizione originale
void Chessboard::resetPezzo()
{
	if (pezzoMosso.Riga() != riga || pezzoMosso.Col() != col)
		sndPlaySound(L"sounds/illegal.wav", SND_ASYNC);
	pezzi[pezzoMosso.Riga()][pezzoMosso.Col()] = pezzoMosso;
	pezzoMosso = Piece();
	reSottoScacco = Piece();
	arroccoTmp = false;
}
// questo metodo permette di cambiare la pedina in un nuovo pezzo scelto dal giocatore
void Chessboard::CambiaPedina(int _mX, int _mY, bool _mPressed)
{
	fromRowCol2XY(riga, col, x, y);
	DrawRectangle(x, y + 1, CELL_WIDTH, CELL_HEIGHT - 1, LightGray, true);
	if (riga == 0)
		Draw("images/chose_" + colori[0] + "piece.bmp", x, y);
	else if (riga == ROW - 1)
		Draw("images/chose_" + colori[1] + "piece.bmp", x, y);
	// regina
	if (_mX > x && _mX < x + (CELL_WIDTH / 2) && _mY > y && _mY < y + (CELL_HEIGHT / 2))
		DrawRectangle(x, y, (CELL_WIDTH / 2) + 1, (CELL_HEIGHT / 2) + 1, Red, false);
	// torre
	else if (_mX > x + (CELL_WIDTH / 2) && _mX < x + CELL_WIDTH && _mY > y && _mY < y + (CELL_HEIGHT / 2))
		DrawRectangle(x + (CELL_WIDTH / 2), y, (CELL_WIDTH / 2) + 1, (CELL_HEIGHT / 2) + 1, Red, false);
	// alfiere
	else if (_mX > x && _mX < x + (CELL_WIDTH / 2) && _mY > y + (CELL_HEIGHT / 2) && _mY < y + CELL_HEIGHT)
		DrawRectangle(x, y + (CELL_HEIGHT / 2), (CELL_WIDTH / 2) + 1, (CELL_HEIGHT / 2) + 1, Red, false);
	// cavallo
	else if (_mX > x + (CELL_WIDTH / 2) && _mX < x + CELL_WIDTH && _mY > y + (CELL_HEIGHT / 2) && _mY < y + CELL_HEIGHT)
		DrawRectangle(x + (CELL_WIDTH / 2), y + (CELL_HEIGHT / 2), (CELL_WIDTH / 2) + 1, (CELL_HEIGHT) / 2 + 1, Red, false);
	// quando clicca con il mouse controllo se ha selezionato un pezzo all'interno del quadrato
	if (_mPressed)
	{
		if (riga == 0)
		{
			// regina
			if (_mX > x && _mX < x + (CELL_WIDTH / 2) && _mY > y && _mY < y + (CELL_HEIGHT / 2))
				pezzi[riga][col] = Piece(colori[0] + "queen" + imgExt, riga, col);
			// torre
			else if (_mX > x + (CELL_WIDTH / 2) && _mX < x + CELL_WIDTH && _mY > y && _mY < y + (CELL_HEIGHT / 2))
				pezzi[riga][col] = Piece(colori[0] + "rook" + imgExt, riga, col);
			// alfiere
			else if (_mX > x && _mX < x + (CELL_WIDTH / 2) && _mY > y + (CELL_HEIGHT / 2) && _mY < y + CELL_HEIGHT)
				pezzi[riga][col] = Piece(colori[0] + "bishop" + imgExt, riga, col);
			// cavallo
			else if (_mX > x + (CELL_WIDTH / 2) && _mX < x + CELL_WIDTH && _mY > y + (CELL_HEIGHT / 2) && _mY < y + CELL_HEIGHT)
				pezzi[riga][col] = Piece(colori[0] + "knight" + imgExt, riga, col);
		}
		else if (riga == ROW - 1)
		{
			// regina
			if (_mX > x && _mX < x + (CELL_WIDTH / 2) && _mY > y && _mY < y + (CELL_HEIGHT / 2))
				pezzi[riga][col] = Piece(colori[1] + "queen" + imgExt, riga, col);
			// torre
			else if (_mX > x + (CELL_WIDTH / 2) && _mX < x + CELL_WIDTH && _mY > y && _mY < y + (CELL_HEIGHT / 2))
				pezzi[riga][col] = Piece(colori[1] + "rook" + imgExt, riga, col);
			// alfiere
			else if (_mX > x && _mX < x + (CELL_WIDTH / 2) && _mY > y + (CELL_HEIGHT / 2) && _mY < y + CELL_HEIGHT)
				pezzi[riga][col] = Piece(colori[1] + "bishop" + imgExt, riga, col);
			// cavallo
			else if (_mX > x + (CELL_WIDTH / 2) && _mX < x + CELL_WIDTH && _mY > y + (CELL_HEIGHT / 2) && _mY < y + CELL_HEIGHT)
				pezzi[riga][col] = Piece(colori[1] + "knight" + imgExt, riga, col);
		}
		if (!pezzi[riga][col].Is("pawn") && pezzi[riga][col].Exist())
			pezzoMosso.setPromuovi(false);
	}
	Present();
}
// grazie a questo metodo riesco ad ottenere la riga e la colonna del campo passandogli le coordinate del mouse
void Chessboard::fromXY2RowCol(int _x, int _y, int& _row, int& _col)
{
	_col = _x / (CELL_WIDTH);
	_row = _y / (CELL_HEIGHT);
}
// con questo metodo ottengo invece le coordinate iniziali della cella avendo solamente la riga e la colonna del campo
void Chessboard::fromRowCol2XY(int _row, int _col, int& _x, int& _y)
{
	_x = _col * CELL_WIDTH;
	_y = _row * CELL_HEIGHT;
}
// controllo se due pezzi hanno lo stesso colore
bool Chessboard::StessoColore(Piece p1, Piece p2)
{
	return (p1.Is("white") && p2.Is("white")) || (p1.Is("black") && p2.Is("black"));
}
// controllo se il re può arroccare corto, quindi controllo se il percorso è vuoto
bool Chessboard::ArroccoDxVuoto(Piece pMoved)
{
	return (// innanzitutto controllo che il pezzo mosso sia un re e non si è ancora mosso per la prima volta
		!pMoved.PrimaMossa() && pMoved.Is("king"))
		// poi controllo se il colore che sta in basso è il bianco
		&& ((colori[0]._Equal("white_")
			// e vado a verificare che non ci siano pezzi nella direzione in cui si sta muovendo, che possano
			// ostacolarlo
			&& !pezzi[pMoved.Riga()][pMoved.Col() + 1].Exist() && !pezzi[pMoved.Riga()][pMoved.Col() + 2].Exist())
			// mentre se il colore che sta sotto è il nero i controlli saranno differenti
			// perché il re e la regina si scambiano quindi il lato lungo diventa e viceversa
			|| (colori[0]._Equal("black_")
				// controllo anche qua che non ci siano pezzi sul percorso
				&& !pezzi[pMoved.Riga()][pMoved.Col() + 1].Exist() && !pezzi[pMoved.Riga()][pMoved.Col() + 2].Exist()
				&& !pezzi[pMoved.Riga()][pMoved.Col() + 3].Exist()));
}
// controllo se il re può arroccare lungo, quindi controllo se il percorso è vuoto
bool Chessboard::ArroccoSxVuoto(Piece pMoved)
{
	return (// innanzitutto controllo che il pezzo mosso sia un re e non si è ancora mosso per la prima volta
		!pMoved.PrimaMossa() && pMoved.Is("king"))
		// poi controllo se il colore che sta in basso è il bianco
		&& ((colori[0]._Equal("white_")
			// e vado a verificare che non ci siano pezzi nella direzione in cui si sta muovendo, che possano
			// ostacolarlo
			&& !pezzi[pMoved.Riga()][pMoved.Col() - 1].Exist() && !pezzi[pMoved.Riga()][pMoved.Col() - 2].Exist()
			&& !pezzi[pMoved.Riga()][pMoved.Col() - 3].Exist())
			// mentre se il colore che sta sotto è il nero i controlli saranno differenti
			// perché il re e la regina si scambiano quindi il lato lungo diventa e viceversa
			|| (colori[0]._Equal("black_")
				// controllo anche qua che non ci siano pezzi sul percorso
				&& !pezzi[pMoved.Riga()][pMoved.Col() - 1].Exist() && !pezzi[pMoved.Riga()][pMoved.Col() - 2].Exist()));
}
// questo metodo serve per disegnare tutte le posizioni possibili del pezzo selezionato
void Chessboard::DisegnaMosseDisponibili()
{
	double count = 1;
	Piece piece = Piece();
	fromRowCol2XY(pezzoMosso.Riga(), pezzoMosso.Col(), x, y);
	// controllo le mosse per la torre e la regina
	if (pezzoMosso.Is("rook") || pezzoMosso.Is("queen"))
	{
		count = 1.5; // a destra
		for (int i = pezzoMosso.Col() + 1; i < COLUMN; i++, count++)
		{
			if (pezzi[pezzoMosso.Riga()][i].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[pezzoMosso.Riga()][i]))
					break;
				else
					DrawCircle(x + (CELL_WIDTH * count), y + (CELL_HEIGHT / 2), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x + (CELL_WIDTH * count), y + (CELL_HEIGHT / 2), CELL_WIDTH / 7, LightGray, true);
		}
		count = 0.5; // a sinistra
		for (int i = pezzoMosso.Col() - 1; i >= 0; i--, count++)
		{
			if (pezzi[pezzoMosso.Riga()][i].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[pezzoMosso.Riga()][i]))
					break;
				else
					DrawCircle(x - (CELL_WIDTH * count), y + (CELL_HEIGHT / 2), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x - (CELL_WIDTH * count), y + (CELL_HEIGHT / 2), CELL_WIDTH / 7, LightGray, true);
		}
		count = 1.5; // in basso
		for (int i = pezzoMosso.Riga() + 1; i < ROW; i++, count++)
		{
			if (pezzi[i][pezzoMosso.Col()].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[i][pezzoMosso.Col()]))
					break;
				else
					DrawCircle(x + (CELL_WIDTH / 2), y + (CELL_HEIGHT * count), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x + (CELL_WIDTH / 2), y + (CELL_HEIGHT * count), CELL_WIDTH / 7, LightGray, true);
		}
		count = 0.5; // in alto
		for (int i = pezzoMosso.Riga() - 1; i >= 0; i--, count++)
		{
			if (pezzi[i][pezzoMosso.Col()].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[i][pezzoMosso.Col()]))
					break;
				else
					DrawCircle(x + (CELL_WIDTH / 2), y - (CELL_HEIGHT * count), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x + (CELL_WIDTH / 2), y - (CELL_HEIGHT * count), CELL_WIDTH / 7, LightGray, true);
		}
	}
	// controllo le mosse per l'alfiere e la regina
	if (pezzoMosso.Is("bishop") || pezzoMosso.Is("queen"))
	{
		count = 1.5; // in basso a destra
		for (int r = pezzoMosso.Riga() + 1, c = pezzoMosso.Col() + 1; r < ROW && c < COLUMN; r++, c++, count++)
		{
			if (pezzi[r][c].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[r][c]))
					break;
				else
					DrawCircle(x + (CELL_WIDTH * count), y + (CELL_HEIGHT * count), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x + (CELL_WIDTH * count), y + (CELL_HEIGHT * count), CELL_WIDTH / 7, LightGray, true);
		}
		count = 1.5; // in basso a sinistra
		for (int r = pezzoMosso.Riga() + 1, c = pezzoMosso.Col() - 1; r < ROW && c >= 0; r++, c--, count++)
		{
			if (pezzi[r][c].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[r][c]))
					break;
				else
					DrawCircle(x - (CELL_WIDTH * (count - 1)), y + (CELL_HEIGHT * count), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x - (CELL_WIDTH * (count - 1)), y + (CELL_HEIGHT * count), CELL_WIDTH / 7, LightGray, true);
		}
		count = 0.5; // in alto a sinistra
		for (int r = pezzoMosso.Riga() - 1, c = pezzoMosso.Col() - 1; r >= 0 && c >= 0; r--, c--, count++)
		{
			if (pezzi[r][c].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[r][c]))
					break;
				else
					DrawCircle(x - (CELL_WIDTH * count), y - (CELL_HEIGHT * count), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x - (CELL_WIDTH * count), y - (CELL_HEIGHT * count), CELL_WIDTH / 7, LightGray, true);
		}
		count = 0.5; // in alto a destra
		for (int r = pezzoMosso.Riga() - 1, c = pezzoMosso.Col() + 1; r >= 0 && c < COLUMN; r--, c++, count++)
		{
			if (pezzi[r][c].Exist())
			{
				if (StessoColore(pezzoMosso, pezzi[r][c]))
					break;
				else
					DrawCircle(x + (CELL_WIDTH * (count + 1)), y - (CELL_HEIGHT * count), CELL_WIDTH / 2, EatColor, false);
				break;
			}
			else
				DrawCircle(x + (CELL_WIDTH * (count + 1)), y - (CELL_HEIGHT * count), CELL_WIDTH / 7, LightGray, true);
		}
	}
	// controllo le mosse per il cavallo
	if (pezzoMosso.Is("knight"))
	{
		/*----------------------------------------------------------------------*/
		if (pezzoMosso.Riga() < ROW - 2 && pezzoMosso.Col() < COLUMN - 1)
		{
			piece = pezzi[pezzoMosso.Riga() + 2][pezzoMosso.Col() + 1];
			if (!piece.Exist())
				DrawCircle(x + (CELL_WIDTH * 1.5), y + (CELL_HEIGHT * 2.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_WIDTH * 1.5), y + (CELL_HEIGHT * 2.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() < ROW - 2 && pezzoMosso.Col() >= 1)
		{
			piece = pezzi[pezzoMosso.Riga() + 2][pezzoMosso.Col() - 1];
			if (!piece.Exist())
				DrawCircle(x - (CELL_WIDTH / 2), y + (CELL_HEIGHT * 2.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_WIDTH / 2), y + (CELL_HEIGHT * 2.5), CELL_WIDTH / 2, EatColor, false);
		}
		/*----------------------------------------------------------------------*/
		if (pezzoMosso.Riga() >= 2 && pezzoMosso.Col() < COLUMN - 1)
		{
			piece = pezzi[pezzoMosso.Riga() - 2][pezzoMosso.Col() + 1];
			if (!piece.Exist())
				DrawCircle(x + (CELL_WIDTH * 1.5), y - (CELL_HEIGHT * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_WIDTH * 1.5), y - (CELL_HEIGHT * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() >= 2 && pezzoMosso.Col() >= 1)
		{
			piece = pezzi[pezzoMosso.Riga() - 2][pezzoMosso.Col() - 1];
			if (!piece.Exist())
				DrawCircle(x - (CELL_WIDTH / 2), y - (CELL_HEIGHT * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_WIDTH / 2), y - (CELL_HEIGHT * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		/*----------------------------------------------------------------------*/
		if (pezzoMosso.Riga() < ROW - 1 && pezzoMosso.Col() < COLUMN - 2)
		{
			piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col() + 2];
			if (!piece.Exist())
				DrawCircle(x + (CELL_WIDTH * 2.5), y + (CELL_HEIGHT * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_WIDTH * 2.5), y + (CELL_HEIGHT * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() >= 1 && pezzoMosso.Col() < COLUMN - 2)
		{
			piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col() + 2];
			if (!piece.Exist())
				DrawCircle(x + (CELL_WIDTH * 2.5), y - (CELL_HEIGHT / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_WIDTH * 2.5), y - (CELL_HEIGHT / 2), CELL_WIDTH / 2, EatColor, false);
		}
		/*----------------------------------------------------------------------*/
		if (pezzoMosso.Riga() < ROW - 1 && pezzoMosso.Col() >= 2)
		{
			piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col() - 2];
			if (!piece.Exist())
				DrawCircle(x - (CELL_WIDTH * 1.5), y + (CELL_HEIGHT * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_WIDTH * 1.5), y + (CELL_HEIGHT * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() >= 1 && pezzoMosso.Col() >= 2)
		{
			piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col() - 2];
			if (!piece.Exist())
				DrawCircle(x - (CELL_WIDTH * 1.5), y - (CELL_HEIGHT / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_WIDTH * 1.5), y - (CELL_HEIGHT / 2), CELL_WIDTH / 2, EatColor, false);
		}
		/*----------------------------------------------------------------------*/
	}
	// controllo le mosse per il re
	if (pezzoMosso.Is("king"))
	{
		if (pezzoMosso.Riga() >= 1 && pezzoMosso.Col() >= 1)
		{
			piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col() - 1];	// in alto a sinistra
			if (!piece.Exist())
				DrawCircle(x - (CELL_WIDTH / 2), y - (CELL_HEIGHT / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_WIDTH / 2), y - (CELL_HEIGHT / 2), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() >= 1)
		{
			piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col()]; // in alto al centro
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT / 2), y - (CELL_WIDTH / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_HEIGHT / 2), y - (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() >= 1 && pezzoMosso.Col() < COLUMN - 1)
		{
			piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col() + 1]; // in alto a destra
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT * 1.5), y - (CELL_WIDTH / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_HEIGHT * 1.5), y - (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Col() < COLUMN - 1)
		{
			piece = pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 1]; // in mezzo a destra
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Col() < COLUMN - 1 && pezzoMosso.Riga() < ROW - 1)
		{
			piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col() + 1]; // in basso a destra
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() < ROW - 1)
		{
			piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col()]; // in basso al centro
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x + (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Riga() < ROW - 1 && pezzoMosso.Col() >= 1)
		{
			piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col() - 1]; // in basso a sinistra
			if (!piece.Exist())
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 2, EatColor, false);
		}
		if (pezzoMosso.Col() >= 1)
		{
			piece = pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 1]; // in mezzo a sinistra
			if (!piece.Exist())
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 7, LightGray, true);
			else if (piece.Exist() && !StessoColore(pezzoMosso, piece))
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
		}
		// controllo anche per l'arrocco
		if (!pezzoMosso.Arrocco())
		{
			if (ArroccoDxVuoto(pezzoMosso))
			{
				if (colori[0]._Equal("white_") && !pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 3].PrimaMossa()
					&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 3].Is("rook"))
				{
					DrawCircle(x + (CELL_WIDTH * 2) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
					DrawCircle(x + (CELL_WIDTH * 3) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, DarkGray, false);
				}
				else if (colori[0]._Equal("black_") && !pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 4].PrimaMossa()
					&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 4].Is("rook"))
				{
					DrawCircle(x + (CELL_WIDTH * 2) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
					DrawCircle(x + (CELL_WIDTH * 4) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, DarkGray, false);
				}
			}
			if (ArroccoSxVuoto(pezzoMosso))
			{
				if (colori[0]._Equal("white_") && !pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 4].PrimaMossa()
					&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 4].Is("rook"))
				{
					DrawCircle(x - (CELL_WIDTH * 2) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
					DrawCircle(x - (CELL_WIDTH * 4) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, DarkGray, false);
				}
				else if (colori[0]._Equal("black_") && !pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 3].PrimaMossa()
					&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 3].Is("rook"))
				{
					DrawCircle(x - (CELL_WIDTH * 2) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
					DrawCircle(x - (CELL_WIDTH * 3) + (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, DarkGray, false);
				}
			}
		}
	}
	// controllo infine le mosse per le pedine
	if (pezzoMosso.Is("pawn"))
	{
		if ((pezzoMosso.Is("white") && colori[0]._Equal("white_")) || pezzoMosso.Is("black") && colori[0]._Equal("black_"))
		{
			piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col()];
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT / 2), y - (CELL_WIDTH / 2), CELL_WIDTH / 7, LightGray, true);
			if (pezzoMosso.Col() >= 1)
			{
				piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col() - 1];
				if (piece.Exist() && !StessoColore(pezzoMosso, piece))
					DrawCircle(x - (CELL_HEIGHT / 2), y - (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
			}
			if (pezzoMosso.Col() < COLUMN - 1)
			{
				piece = pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col() + 1];
				if (piece.Exist() && !StessoColore(pezzoMosso, piece))
					DrawCircle(x + (CELL_HEIGHT * 1.5), y - (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
			}
			if (!pezzoMosso.PrimaMossa())
			{
				piece = pezzi[pezzoMosso.Riga() - 2][pezzoMosso.Col()];
				if (!piece.Exist() && !pezzi[pezzoMosso.Riga() - 1][pezzoMosso.Col()].Exist())
					DrawCircle(x + (CELL_HEIGHT / 2), y - (CELL_WIDTH * 2) + (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
			}
			if (pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 1].Is(colori[1] + "pawn")
				&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 1].EnPassant() && twoFAenPass)
			{
				DrawCircle(x - (CELL_HEIGHT / 2), y - (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
			}
			else if (pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 1].Is(colori[1] + "pawn")
				&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 1].EnPassant() && twoFAenPass)
			{
				DrawCircle(x + (CELL_HEIGHT * 1.5), y - (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
			}
		}
		else if ((pezzoMosso.Is("black") && colori[1]._Equal("black_")) || pezzoMosso.Is("white") && colori[1]._Equal("white_"))
		{
			piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col()];
			if (!piece.Exist())
				DrawCircle(x + (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 7, LightGray, true);
			if (pezzoMosso.Col() >= 1)
			{
				piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col() - 1];
				if (piece.Exist() && !StessoColore(pezzoMosso, piece))
					DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 2, EatColor, false);
			}
			if (pezzoMosso.Col() < COLUMN - 1)
			{
				piece = pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col() + 1];
				if (piece.Exist() && !StessoColore(pezzoMosso, piece))
					DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 2, EatColor, false);
			}
			if (!pezzoMosso.PrimaMossa())
			{
				piece = pezzi[pezzoMosso.Riga() + 2][pezzoMosso.Col()];
				if (!piece.Exist() && !pezzi[pezzoMosso.Riga() + 1][pezzoMosso.Col()].Exist())
					DrawCircle(x + (CELL_HEIGHT / 2), y + (CELL_WIDTH * 2) + (CELL_WIDTH / 2), CELL_WIDTH / 7, DarkGray, true);
			}
			if (pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 1].Is(colori[0] + "pawn")
				&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() - 1].EnPassant() && twoFAenPass)
			{
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 7, DarkGray, true);
				DrawCircle(x - (CELL_HEIGHT / 2), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
			}
			else if (pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 1].Is(colori[0] + "pawn")
				&& pezzi[pezzoMosso.Riga()][pezzoMosso.Col() + 1].EnPassant() && twoFAenPass)
			{
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH * 1.5), CELL_WIDTH / 7, DarkGray, true);
				DrawCircle(x + (CELL_HEIGHT * 1.5), y + (CELL_WIDTH / 2), CELL_WIDTH / 2, EatColor, false);
			}
		}
	}
}
