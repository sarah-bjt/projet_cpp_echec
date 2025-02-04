#include <board.hpp>

int ChessBoard::position_to_index(int x, int y) const
{
    return y * 8 + x;
}