#include <iostream>
#include <vector>
#include "Piece.hpp"

class ChessBoard {
private:
    std::vector<Piece*> board; // Tableau 1D de 64 cases

public:
    ChessBoard()
        : board(64, nullptr) {} // Initialise un échiquier vide

    int position_to_index(int x, int y) const;
};