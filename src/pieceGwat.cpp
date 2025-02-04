#include "piece.hpp"

Piece::Piece(const std::string& name, const std::string& color, const std::pair<int, int>& startPosition)
    : type(std::make_pair(name, color)), position(startPosition), state(State::Alive) {
}

