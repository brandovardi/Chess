#include "StartGame.h"

string StartGame::chooseColor()
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

string StartGame::controllaSceltaColore(int mx, int my)
{
	if (mx > ((Width / 2) - CELL_WIDTH) && mx < (Width / 2) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		return "white";
	else if (mx > (Width / 2) && mx < ((Width / 2) + CELL_WIDTH) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		return "black";
	return "";
}

void StartGame::drawSquare(int mx, int my)
{
	if (mx > ((Width / 2) - CELL_WIDTH) && mx < (Width / 2) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		DrawRectangle(((Width / 2) - CELL_WIDTH), CELL_HEIGHT, CELL_WIDTH - 1, CELL_HEIGHT, Black, false);
	else if (mx > (Width / 2) && mx < ((Width / 2) + CELL_WIDTH) && my > CELL_HEIGHT && my < (CELL_HEIGHT * 2))
		DrawRectangle((Width / 2), CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, Black, false);
}
