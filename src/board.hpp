#include <iostream>
#include <vector>
#include "Piece.hpp"

class ChessBoard {
private:
    std::vector<Piece*> board;

public:
    ChessBoard() : board(64, nullptr) {}

    int position_to_index(int x, int y) const {
        return y * 8 + x;
    }
}