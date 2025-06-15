#include "overseer.h"
#include "resource1.h"
#include "tile.h"

void Overseer::SetGame() {
	playerTurn = WHITE;
	selectTile = { -1, -1 };
	msgTurn = "White player turn";
	msgPlayer = "";
	SetBoard();
}

void Overseer::SetBoard() {
	for (int y = 0; y < BOARDSIZE; y++) {
		for (int x = 0; x < BOARDSIZE; x++) {
			board[x][y].SetCoord(x, y);
			board[x][y].SetTile(NONE, NONE);
			if (y == 0 || y == 1 || y == BOARDSIZE - 2 || y == BOARDSIZE - 1) {
				board[x][y].SetSpecial(true);
			}
		}
	}
	
	for (int x = 0; x < BOARDSIZE; x++) {
		board[x][1].SetTile(PAWN, BLACK);
		board[x][BOARDSIZE - 2].SetTile(PAWN, WHITE);
	}

	board[0][0].SetTile(ROOK, BLACK);
	board[BOARDSIZE - 1][0].SetTile(ROOK, BLACK);
	board[0][BOARDSIZE - 1].SetTile(ROOK, WHITE);
	board[BOARDSIZE - 1][BOARDSIZE - 1].SetTile(ROOK, WHITE);

	board[1][0].SetTile(KNIGHT, BLACK);
	board[BOARDSIZE - 2][0].SetTile(KNIGHT, BLACK);
	board[1][BOARDSIZE - 1].SetTile(KNIGHT, WHITE);
	board[BOARDSIZE - 2][BOARDSIZE - 1].SetTile(KNIGHT, WHITE);

	board[BOARDSIZE - 6][0].SetTile(BISHOP, BLACK);
	board[BOARDSIZE - 3][0].SetTile(BISHOP, BLACK);
	board[BOARDSIZE - 6][BOARDSIZE - 1].SetTile(BISHOP, WHITE);
	board[BOARDSIZE - 3][BOARDSIZE - 1].SetTile(BISHOP, WHITE);

	board[BOARDSIZE - 5][0].SetTile(QUEEN, BLACK);
	board[BOARDSIZE - 4][0].SetTile(KING, BLACK);

	board[BOARDSIZE - 5][BOARDSIZE - 1].SetTile(QUEEN, WHITE);
	board[BOARDSIZE - 4][BOARDSIZE - 1].SetTile(KING, WHITE);

	ManageThreat();
}

void Overseer::GameLoop() {
	int outcome;
	if (playerTurn == WHITE) {
		msgTurn = "Black player turn";
		playerTurn = BLACK;
		outcome = Checkmate();
		if (outcome == 1) {
			msgTurn = "Game over! White player won!";
		}
	}
	else {
		msgTurn = "White player turn";
		playerTurn = WHITE;
		outcome = Checkmate();
		if (outcome == 1) {
			msgTurn = "Game over! Black player won!";
		}
	}
	if (outcome == 2) {
		msgTurn = "Draw! Player can't make any moves!";
	}
}

void Overseer::DisplayMessage(HWND hWnd, HDC hdc, HINSTANCE hInst) {
	RECT window;
	GetWindowRect(hWnd, &window);
	TextOut(hdc, (window.right - window.left) / 2 - TILESIZE, 30, "                                                                                                   ", 100);
	TextOut(hdc, (window.right - window.left)/ 2 - TILESIZE, 30, msgTurn, lstrlen(msgTurn));
	TextOut(hdc, 0, 0, "                                                                                                   ", 100);
	TextOut(hdc, 0, 0, msgPlayer, lstrlen(msgPlayer));
}

void Overseer::DrawBoard(HWND hWnd, HDC hdc, HINSTANCE hInst) {
	RECT window;
	GetWindowRect(hWnd, &window);
	startH = ((window.right - window.left) - BOARDSIZE * TILESIZE) / 2;
	startV = 100;
	stopH = startH + BOARDSIZE * TILESIZE;
	stopV = startV + BOARDSIZE * TILESIZE;

	bool black = 0;
	HBRUSH blackBrush = (HBRUSH)CreateSolidBrush(CLR_BLACK);
	HBRUSH whiteBrush = (HBRUSH)CreateSolidBrush(CLR_WHITE);

	HBITMAP bmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FIGURES));
	HDC MemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(MemDC, bmp);

	LPCSTR text[2][8] = { {"8", "7", "6", "5", "4", "3", "2", "1"},
	{"A", "B", "C", "D", "E", "F", "G", "H"} };
	for (int y = startV; y < stopV; y += TILESIZE) {
		int boardY = (y - startV) / TILESIZE;
		for (int x = startH; x < stopH; x += TILESIZE)
		{
			int boardX = (x - startH) / TILESIZE;
			if (black) {
				SelectObject(hdc, blackBrush);
			}
			else {
				SelectObject(hdc, whiteBrush);
			}
			Rectangle(hdc, x, y, x + TILESIZE, y + TILESIZE);
			black = !black;

			TextOut(hdc, x + 30, startV - 30,
				text[1][boardX], 1);
			TextOut(hdc, x + 30, startV + BOARDSIZE * TILESIZE + 30,
				text[1][boardX], 1);

			if (board[boardX][boardY].GetColor()) { //draw figures
				int offsetX = (board[boardX][boardY].GetFigure() - 1) * BMPSIZE;
				int offsetY;
				if (board[boardX][boardY].GetColor() == BLACK) {
					offsetY = 0 * BMPSIZE;
				}
				else {
					offsetY = 1 * BMPSIZE;
				}
				BitBlt(hdc, x + (TILESIZE-BMPSIZE) / 2, y + (TILESIZE - BMPSIZE) / 2,
					BMPSIZE, BMPSIZE, MemDC,
					offsetX, offsetY, SRCCOPY);
			}

			if (boardX == selectTile.x && boardY == selectTile.y) { //draw circle over selected tiles
				SetROP2(hdc, R2_NOTXORPEN);
				Arc(hdc, x, y, x + TILESIZE, y + TILESIZE, 0, 0, 0, 0);
				SetROP2(hdc, 13);
			}
		}
		black = !black;
		TextOut(hdc, startH - 30, y + 30, 
			text[0][boardY], 1);
		TextOut(hdc, startH + BOARDSIZE * TILESIZE + 30, y + 30,
			text[0][boardY], 1);
	}
	DeleteObject(blackBrush);
	DeleteObject(whiteBrush);
}

void Overseer::OnLBclick(HWND hWnd) {
	GetCursorPos(&LBclick);
	ScreenToClient(hWnd, &LBclick);

	if (LBclick.x > startH && LBclick.x < stopH
		&& LBclick.y > startV && LBclick.y < stopV) {
		LBclick = ScreenToBoard(LBclick);
		PickTile(LBclick);
	}

	InvalidateRect(hWnd, 0, 0);
}

void Overseer::PickTile(POINT coord) {}

bool Overseer::MakeTurn(POINT coord) {
	return 0;
}

void Overseer::ManageThreat() {}

int Overseer::Checkmate() {
	return 0;
}

Tile* Overseer::FindKing(int clr) {
	return nullptr;
}

void Overseer::ResetGame(WPARAM wParam) {
	if (key_input) {
		switch (wParam) {
			case 'R':
				SetGame();
				break;
		}
	}
}

POINT Overseer::ScreenToBoard(POINT coord) {
	POINT tmp;
	tmp.x = (coord.x - startH) / TILESIZE;
	tmp.y = (coord.y - startV) / TILESIZE;
	return tmp;
}