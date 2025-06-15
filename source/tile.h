#pragma once

#include "framework.h"

#define BOARDSIZE 8

enum COLORS { NONE = 0, BLACK = -1, WHITE = 1};

enum FIGURES { KING = 1, QUEEN, ROOK, KNIGHT, BISHOP, PAWN };

class Tile {
protected:
	int fig = NONE;
	int clr = NONE;
	bool spcl = false;
	bool threats[3] = { false, false, false }; // BLACK,,WHITE
	POINT coord;
	POINT coord_prev;
public:
	Tile(int x, int y);
	Tile() : Tile(-1, -1) {};
	~Tile();
	Tile operator=(const Tile& new_tile);
	void SetCoord(int x, int y);
	void SetPrev(int x, int y);
	void SetTile(int new_fig, int new_clr);
	void SetSpecial(bool new_spcl);
	void SetThreat(bool new_threat, int index);
	POINT GetCoord();
	POINT GetPrev();
	int GetColor();
	int GetFigure();
	bool GetSpecial();
	bool GetThreat(int index);
	void MovePiece(Tile* new_tile);
	void SpreadThreat(Tile board[][BOARDSIZE]);
	void ThreatHor(Tile board[][BOARDSIZE]);
	void ThreatVert(Tile board[][BOARDSIZE]);
	void ThreatDiag(Tile board[][BOARDSIZE]);
	bool IsMoveAllowed(Tile board[][BOARDSIZE], Tile* dest);
	bool IsHorClear(Tile board[][BOARDSIZE], Tile* dest);
	bool IsVertClear(Tile board[][BOARDSIZE], Tile* dest);
	bool IsDiagClear(Tile board[][BOARDSIZE], Tile* dest);
};