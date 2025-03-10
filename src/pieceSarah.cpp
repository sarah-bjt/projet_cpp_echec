// #include <iostream>
// #include <piece.hpp>

// Piece::Piece(const std::string& name, int color, const std::pair<int, int>& startPosition)
// {
//     type     = std::make_pair(name, color);
//     position = startPosition;
//     state    = State::Alive;
// }

// Piece::possible_moves(const auto type){
//     std::vector<Piece::position> possible_position_table;
//     if (type.first==Name::Rock){
//         possible_position_table.
//     }
// }

#include "piece.hpp"

// Constructeur
Piece::Piece(const std::string& nameStr, const std::string& colorStr, const std::pair<int, int>& startPosition)
    : position(startPosition), state(State::Alive)
{ // Par défaut, une pièce est en vie

    // Conversion du nom
    if (nameStr == "Rook")
        name = Name::Rook;
    else if (nameStr == "Queen")
        name = Name::Queen;
    else if (nameStr == "King")
        name = Name::King;

    // Conversion de la couleur
    if (colorStr == "Black")
        color = Color::Black;
    else if (colorStr == "White")
        color = Color::White;
}

void Piece::move(const std::pair<int, int>& newPosition)
{
    position = newPosition;
}

std::pair<int, int> Piece::get_position() const
{
    return position;
}

std::string Piece::get_type() const
{
    switch (name)
    {
    case Name::Rook: return "Rook";
    case Name::Queen: return "Queen";
    case Name::King: return "King";
    default: return "Unknown";
    }
}

Piece::State Piece::get_state() const
{
    return state;
}

void Piece::set_state(State newState)
{
    state = newState;
}

std::vector<std::pair<int, int>> Piece::possible_moves(const std::string& type) const
{
    std::vector<std::pair<int, int>> moves;

    if (type == "Rook")
    {
        // Déplacements en ligne droite (horizontaux et verticaux)
        for (int i = 1; i < 8; ++i)
        {
            moves.push_back({position.first + i, position.second});
            moves.push_back({position.first - i, position.second});
            moves.push_back({position.first, position.second + i});
            moves.push_back({position.first, position.second - i});
        }
    }

    else if (type == "Queen")
    {
        // Combine les mouvements de la tour et du fou
        for (int i = 1; i < 8; ++i)
        {
            moves.push_back({position.first + i, position.second});
            moves.push_back({position.first - i, position.second});
            moves.push_back({position.first, position.second + i});
            moves.push_back({position.first, position.second - i});
            moves.push_back({position.first + i, position.second + i});
            moves.push_back({position.first - i, position.second - i});
            moves.push_back({position.first + i, position.second - i});
            moves.push_back({position.first - i, position.second + i});
        }
    }
    else if (type == "King")
    {
        // Déplacements d'une case dans toutes les directions
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int i = 0; i < 8; ++i)
        {
            moves.push_back({position.first + dx[i], position.second + dy[i]});
        }
    }

    return moves;
}
