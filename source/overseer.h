#pragma once

#include "framework.h"
#include "tile.h"

#define TILESIZE 80
#define BMPSIZE 48
#define CLR_BLACK RGB(0,0,0)
#define CLR_WHITE RGB(255,255,255)

class Overseer {
protected:
	bool key_input = true;
	int startH, startV, stopH, stopV;
	int playerTurn = WHITE;
	POINT LBclick;
	POINT selectTile = {-1, -1};
	Tile board[BOARDSIZE][BOARDSIZE];
	LPCSTR msgTurn = "White player turn";
	LPCSTR msgPlayer = "";
public:
	void SetGame();
	void SetBoard();
	void GameLoop();
	void DisplayMessage(HWND hWnd, HDC hdc, HINSTANCE hInst);
	void DrawBoard(HWND hWnd, HDC hdc, HINSTANCE hInst);
	void OnLBclick(HWND hWnd);
	void PickTile(POINT coord);
	bool MakeTurn(POINT coord);
	void ManageThreat();
	int Checkmate();
	Tile* FindKing(int clr);
	void ResetGame(WPARAM wParam);
	POINT ScreenToBoard(POINT coord);
};