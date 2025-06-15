#include "tile.h"

Tile::Tile(int x, int y) {
    coord = { x, y };
	coord_prev = coord;
}

Tile::~Tile() {}

Tile Tile::operator=(const Tile& new_tile) {
    coord = new_tile.coord;
    coord_prev = new_tile.coord_prev;
    fig = new_tile.fig;
    clr = new_tile.clr;
    spcl = new_tile.spcl;
    threats[0] = new_tile.threats[0];
    threats[1] = new_tile.threats[1];
    threats[2] = new_tile.threats[2];
    return *this;
}

void Tile::SetCoord(int x, int y) {
    coord = { x, y };
}

void Tile::SetPrev(int x, int y) {
    coord_prev = { x, y };
}

void Tile::SetTile(int new_fig, int new_clr) {
	fig = new_fig;
	clr = new_clr;
}

void Tile::SetSpecial(bool new_spcl) {
    spcl = new_spcl;
}

void Tile::SetThreat(bool new_threat, int index) {
    threats[index] = new_threat;
}

POINT Tile::GetCoord() {
    return coord;
}

POINT Tile::GetPrev() {
    return coord_prev;
}

int Tile::GetColor() {
	return clr;
}

int Tile::GetFigure() {
	return fig;
}

bool Tile::GetSpecial() {
    return spcl;
}

bool Tile::GetThreat(int index) {
    return threats[index];
}

void Tile::MovePiece(Tile* new_tile) {
    new_tile->SetTile(fig, clr);
    new_tile->SetPrev(coord.x, coord.y);
    SetSpecial(false);
    SetTile(NONE, NONE);
}

void Tile::SpreadThreat(Tile board[][BOARDSIZE]) {}

bool Tile::IsMoveAllowed(Tile board[][BOARDSIZE], Tile* dest) {
	return false;
}
