#pragma once

#include <iostream>
#include <string>
#include "drawing.h"
#include "Chessboard.h"

using namespace std;

class StartGame
{
private:

public:
	StartGame() {}
	string controllaSceltaColore(int mx, int my);
	void drawSquare(int mx, int my);
	string chooseColor();
};

