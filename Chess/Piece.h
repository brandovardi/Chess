#pragma once

#include <iostream>
#include <string>
#include <regex> // per controllare il nome del pezzo

using namespace std;

const string IMG_PATH = "images/";

class Piece {
private:
	string nome;
	int riga;
	int colonna;
	bool arrocco;
	bool primaMossa;
	bool promuovi;
	bool enPassant;

public:
	Piece() :
		nome(""),
		riga(-1),
		colonna(-1),
		arrocco(false),
		primaMossa(false),
		promuovi(false),
		enPassant(false)
	{}
	Piece(int r, int c) :
		nome(""),
		riga(r),
		colonna(c),
		arrocco(false),
		primaMossa(false),
		promuovi(false),
		enPassant(false)
	{}
	Piece(string _n, int _r, int _c, bool _fm = false, bool _castling = false, bool promoting = false, bool enPassant = false) :
		nome(_n),
		riga(_r),
		colonna(_c),
		arrocco(_castling),
		primaMossa(_fm),
		promuovi(promoting),
		enPassant(enPassant)
	{}
	~Piece() {}
	// controllo se due pezzi sono uguali
	bool ugualeA(Piece _p) { return (this->riga == _p.riga && this->colonna == _p.colonna && this->nome == _p.nome); }
	// setters
	void setRiga(int val) { this->riga = val; }
	void setCol(int val) { this->colonna = val; }
	void setArrocco(bool val) { this->arrocco = val; }
	void setPrimaMossa(bool val) { this->primaMossa = val; } // principalmente per i pedoni, il re e le torri
	void setPromuovi(bool val) { this->promuovi = val; }
	void setEnPassant(bool val) { this->enPassant = val; }
	// getters
	string Nome() { return this->nome; }
	int Riga() { return this->riga; }
	int Col() { return this->colonna; }
	bool Arrocco() { return this->arrocco; }
	bool PrimaMossa() { return this->primaMossa; }
	bool Promuovi() { return this->promuovi; }
	bool EnPassant() { return this->enPassant; }

	/*
	* La funzione find restituirà std::string::npos se la stringa o il carattere non viene trovato
	* all'interno della stringa di ricerca.
	* In caso contrario, restituirà l'indice iniziale della prima occorrenza trovata.
	*/
	// con questo metodo posso verificare sia se il pezzo è una torre, un cavallo... o anche se è bianco o nero
	bool Is(string piece) { return this->nome.find(piece) != std::string::npos; }

	bool Exist();
	string getServerString();
	string boolToString(bool valore) { return (valore ? "true" : "false"); }
};
