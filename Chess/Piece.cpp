#include "Piece.h"

// controllo se il pezzo esiste andando a controllare che il nome sia stato settato e anche la riga e la colonna
bool Piece::Exist()
{
	// unico pezzo di codice chiesto a Chat Gpt nell'intero programma
	// (la struttura della regex l'ho calcolata online perché non le avevo mai usate prima(ora so più o meno come si usano))
	std::smatch match;
	return std::regex_search(this->nome, match, (regex)"[a-z]+_[a-z]+\\.bmp");
}

string Piece::getServerString()
{
	string str = string(nome + ";" + to_string(riga) + ";" + to_string(colonna));

	str.replace(str.find(".bmp"), 4, "");

	return str;
}
