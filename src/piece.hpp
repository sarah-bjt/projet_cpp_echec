#pragma once
#include <iostream>
#include <vector>

class Piece {
    std::pair<std::string, int> type;
    enum class state {
        dead,
        alive,
    };
    std::pair<int, int> position;
    std::pair<int, int> move();
    std::vector<std::pair<int, int>> possible_move();
};