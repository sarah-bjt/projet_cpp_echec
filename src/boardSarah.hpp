#pragma once
#include <imgui.h>
#include <piece.hpp>

class Board_sarah { // mettre plutôt une struc si tous les trucs sont public
public:
    static void draw_table(Board_sarah& board);
    std::vector<Piece> pieces;
};