// #pragma once
// #include <imgui.h>

// class Board_sarah { // mettre plutôt une struc si tous les trucs sont public
// private:
//     std::vector<std::vector<Piece*>> board;

// public:
//     Board_sarah();
//     static void draw_table();
//     void        place_piece(Piece* piece, int x, int y);
// };

#pragma once
#include <imgui.h>
#include <piece.hpp>


class Board_sarah {
private:
    std::vector<std::vector<Piece*>> board; // Échiquier 8x8 contenant des pointeurs vers les pièces

public:
    static void draw_table();
    std::vector<Piece> pieces;
};
