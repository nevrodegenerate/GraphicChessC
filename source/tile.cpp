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

void Tile::SpreadThreat(Tile board[][BOARDSIZE]) {
    switch (fig) {
    case KING:
        for (int y = max(coord.y - 1, 0); y < min(coord.y + 2, BOARDSIZE - 1); y++) {
            for (int x = max(coord.x - 1, 0); x < min(coord.x + 2, BOARDSIZE - 1); x++) {
                board[x][y].SetThreat(true, clr + 1);
            }
        }
        break;
    case QUEEN:
        ThreatHor(board);
        ThreatVert(board);
        ThreatDiag(board);
        break;
    case ROOK:
        ThreatHor(board);
        ThreatVert(board);
        break;
    case BISHOP:
        ThreatDiag(board);
        break;
    case KNIGHT:
        if (coord.x + 1 < BOARDSIZE && coord.y + 2 < BOARDSIZE)
            board[coord.x + 1][coord.y + 2].SetThreat(true, clr + 1);
        if (coord.x + 2 < BOARDSIZE && coord.y + 1 < BOARDSIZE)
            board[coord.x + 2][coord.y + 1].SetThreat(true, clr + 1);
        if (coord.x - 1 >= 0 && coord.y + 2 < BOARDSIZE)
            board[coord.x - 1][coord.y + 2].SetThreat(true, clr + 1);
        if (coord.x + 2 < BOARDSIZE && coord.y - 1 >= 0)
            board[coord.x + 2][coord.y - 1].SetThreat(true, clr + 1);
        if (coord.x - 2 >= 0 && coord.y + 1 < BOARDSIZE)
            board[coord.x - 2][coord.y + 1].SetThreat(true, clr + 1);
        if (coord.x + 1 < BOARDSIZE && coord.y - 2 >= 0)
            board[coord.x + 1][coord.y - 2].SetThreat(true, clr + 1);
        if (coord.x - 2 >= 0 && coord.y - 1 >= 0)
            board[coord.x - 2][coord.y - 1].SetThreat(true, clr + 1);
        if (coord.x - 1 >= 0 && coord.y - 2 >= 0)
            board[coord.x - 1][coord.y - 2].SetThreat(true, clr + 1);
        break;
    case PAWN:
        if (coord.y - clr) {
            if (coord.x - 1 >= 0)
                board[coord.x - 1][coord.y - clr].SetThreat(true, clr + 1);
            if (coord.x + 1 < BOARDSIZE)
                board[coord.x + 1][coord.y - clr].SetThreat(true, clr + 1);
        }
        break;
    }
}

void Tile::ThreatHor(Tile board[][BOARDSIZE]) {
    for (int x = coord.x - 1; x >= 0; x--) {
        board[x][coord.y].SetThreat(true, clr + 1);
        if (board[x][coord.y].GetColor()) {
            break;
        }
    }
    for (int x = coord.x + 1; x < BOARDSIZE; x++) {
        board[x][coord.y].SetThreat(true, clr + 1);
        if (board[x][coord.y].GetColor()) {
            break;
        }
    }
}

void Tile::ThreatVert(Tile board[][BOARDSIZE]) {
    for (int y = coord.y - 1; y >= 0; y--) {
        board[coord.x][y].SetThreat(true, clr + 1);
        if (board[coord.x][y].GetColor()) {
            break;
        }
    }
    for (int y = coord.y + 1; y < BOARDSIZE; y++) {
        board[coord.x][y].SetThreat(true, clr + 1);
        if (board[coord.x][y].GetColor()) {
            break;
        }
    }
}

void Tile::ThreatDiag(Tile board[][BOARDSIZE]) {
    for (int y = coord.y - 1, x = coord.x - 1; y >= 0 && x >= 0; y--, x--) {
        board[x][y].SetThreat(true, clr + 1);
        if (board[x][y].GetColor()) {
            break;
        }
    }
    for (int y = coord.y + 1, x = coord.x - 1; y < BOARDSIZE && x >= 0; y++, x--) {
        board[x][y].SetThreat(true, clr + 1);
        if (board[x][y].GetColor()) {
            break;
        }
    }
    for (int y = coord.y - 1, x = coord.x + 1; y >= 0 && x < BOARDSIZE; y--, x++) {
        board[x][y].SetThreat(true, clr + 1);
        if (board[x][y].GetColor()) {
            break;
        }
    }
    for (int y = coord.y + 1, x = coord.x + 1; y < BOARDSIZE && x < BOARDSIZE; y++, x++) {
        board[x][y].SetThreat(true, clr + 1);
        if (board[x][y].GetColor()) {
            break;
        }
    }
}

bool Tile::IsMoveAllowed(Tile board[][BOARDSIZE], Tile* dest) {
    if (dest->GetColor() == clr) {
        return false;
    }
    switch (fig) {
    case KING:
        if (coord.x - dest->GetCoord().x <= 1 &&
            coord.x - dest->GetCoord().x >= -1 &&
            coord.y - dest->GetCoord().y <= 1 &&
            coord.y - dest->GetCoord().y >= -1) {
            return true;
        }
        else if (spcl && !GetThreat(1 - clr)) { //castling
            if (dest->GetCoord().x == 2 &&
                board[0][coord.y].GetSpecial()) {
                for (int x = 1; x < coord.x; x++) {
                    if (board[x][coord.y].GetColor() ||
                        board[x][coord.y].GetThreat(1 - clr)) {
                        return false;
                    }
                }
                board[0][coord.y].MovePiece(&board[3][coord.y]);
                return true;
            }
            else if (dest->GetCoord().x == BOARDSIZE - 2 &&
                board[BOARDSIZE - 1][coord.y].GetSpecial()) {
                for (int x = coord.x + 1; x < BOARDSIZE - 1; x++) {
                    if (board[x][coord.y].GetColor() ||
                        board[x][coord.y].GetThreat(1 - clr)) {
                        return false;
                    }
                }
                board[BOARDSIZE - 1][coord.y].MovePiece(&board[BOARDSIZE - 3][coord.y]);
                return true;
            }
        }
        else {
            return false;
        }
        break;
    case QUEEN:
        if (IsVertClear(board, dest) ||
            IsHorClear(board, dest) ||
            IsDiagClear(board, dest)) {
            return true;
        }
        else {
            return false;
        }
        break;
    case ROOK:
        if (IsVertClear(board, dest) ||
            IsHorClear(board, dest)) {
            return true;
        }
        else {
            return false;
        }
        break;
    case BISHOP:
        if (IsDiagClear(board, dest)) {
            return true;
        }
        else {
            return false;
        }
        break;
    case KNIGHT:
        if ((coord.x - dest->GetCoord().x) * (coord.x - dest->GetCoord().x) == 4 &&
            (coord.y - dest->GetCoord().y) * (coord.y - dest->GetCoord().y) == 1 ||
            (coord.x - dest->GetCoord().x) * (coord.x - dest->GetCoord().x) == 1 &&
            (coord.y - dest->GetCoord().y) * (coord.y - dest->GetCoord().y) == 4) {
            return true;
        }
        else {
            return false;
        }
        break;
    case PAWN:
        if (coord.x == dest->GetCoord().x && //move forward 1 space
            coord.y == dest->GetCoord().y + clr &&
            dest->GetFigure() == NONE) {
            return true;
        }
        else if (coord.x == dest->GetCoord().x && //move forward 2 spaces
            coord.y == dest->GetCoord().y + (clr * 2) &&
            dest->GetFigure() == NONE &&
            GetSpecial()) {
            return true;
        }
        else if ((coord.x - dest->GetCoord().x) * (coord.x - dest->GetCoord().x) == 1 && //take enemy figure
            coord.y == dest->GetCoord().y + clr &&
            dest->GetColor() == -clr) {
            return true;
        }
        else if ((coord.x - dest->GetCoord().x) * (coord.x - dest->GetCoord().x) == 1 && //en passaint
            coord.y == dest->GetCoord().y + clr &&
            dest->GetColor() == NONE) {
            Tile* spec_tile = &board[dest->GetCoord().x][dest->GetCoord().y + clr];
            if (spec_tile->GetFigure() == PAWN &&
                spec_tile->GetColor() == -clr &&
                spec_tile->GetPrev().x == spec_tile->GetCoord().x &&
                spec_tile->GetPrev().y == spec_tile->GetCoord().y - (clr * 2)) {
                spec_tile->SetTile(NONE, NONE);
                return true;
            }
            return false;
        }
        else {
            return false;
        }
        break;
    }
    return false;
}

bool Tile::IsHorClear(Tile board[][BOARDSIZE], Tile* dest) {
    if (coord.y == dest->GetCoord().y) {//horizontal
        for (int i = min(coord.x, dest->GetCoord().x) + 1; i < max(coord.x, dest->GetCoord().x); i++) {
            if (board[i][coord.y].GetFigure() != NONE) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Tile::IsVertClear(Tile board[][BOARDSIZE], Tile* dest) {
    if (coord.x == dest->GetCoord().x) {//vertical
        for (int i = min(coord.y, dest->GetCoord().y) + 1; i < max(coord.y, dest->GetCoord().y); i++) {
            if (board[coord.x][i].GetFigure() != NONE) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Tile::IsDiagClear(Tile board[][BOARDSIZE], Tile* dest) {
    if (coord.x < dest->GetCoord().x && //diagonal 1
        coord.y < dest->GetCoord().y &&
        coord.x - dest->GetCoord().x == coord.y - dest->GetCoord().y ||
        coord.x > dest->GetCoord().x && coord.y > dest->GetCoord().y &&
        coord.x - dest->GetCoord().x == coord.y - dest->GetCoord().y) {
        for (int x = min(coord.x, dest->GetCoord().x) + 1, y = min(coord.y, dest->GetCoord().y) + 1;
            x < max(coord.x, dest->GetCoord().x) && y < max(coord.y, dest->GetCoord().y); x++, y++) {
            if (board[x][y].GetFigure() != NONE) {
                return false;
            }
        }
        return true;
    }
    else if (coord.x < dest->GetCoord().x && //diagonal 2
        coord.y > dest->GetCoord().y &&
        coord.x - dest->GetCoord().x == dest->GetCoord().y - coord.y) {
        for (int x = coord.x + 1, y = coord.y - 1; x < dest->GetCoord().x && y > dest->GetCoord().y; x++, y--) {
            if (board[x][y].GetFigure() != NONE) {
                return false;
            }
        }
        return true;
    }
    else if (coord.x > dest->GetCoord().x && //diagonal 3
        coord.y < dest->GetCoord().y &&
        coord.x - dest->GetCoord().x == dest->GetCoord().y - coord.y) {
        for (int x = coord.x - 1, y = coord.y + 1; x > dest->GetCoord().x && y < dest->GetCoord().y; x--, y++) {
            if (board[x][y].GetFigure() != NONE) {
                return false;
            }
        }
        return true;
    }
    return false;
}