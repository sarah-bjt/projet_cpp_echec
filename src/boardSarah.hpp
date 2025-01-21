#pragma once
#include <imgui.h>

class Board_sarah { // mettre plutôt une struc si tous les trucs sont public
public:
    static void draw_royal_line();
    static void draw_pion_line();
    static void draw_vide_line();
    static void draw_board();
    static void draw_table();
};