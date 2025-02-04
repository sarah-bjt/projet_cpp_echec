#include <iostream>
#include <piece.hpp>

Piece::Piece(const std::string& name, int color, const std::pair<int, int>& startPosition)
{
    type     = std::make_pair(name, color);
    position = startPosition;
    state    = State::Alive;
}

Piece::possible_moves(const auto type){
    std::vector<Piece::position> possible_position_table;
    if (type.first==Name::Rock){
        possible_position_table.
    }
}