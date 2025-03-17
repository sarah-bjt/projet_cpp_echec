#include <algorithm>
#include <iostream>
#include <vector>
#include "piece.hpp"

Piece::Piece(const std::string& nameStr, const std::string& colorStr, const std::pair<int, int>& startPosition)
    : position(startPosition), state(State::Alive)
{
    name  = stringToName(nameStr);
    color = stringToColor(colorStr);
}

// Convertir une string en Name
Piece::Name Piece::stringToName(const std::string& nameStr)
{
    if (nameStr == "Rook")
        return Name::Rook;
    if (nameStr == "Knight")
        return Name::Knight;
    if (nameStr == "Bishop")
        return Name::Bishop;
    if (nameStr == "Queen")
        return Name::Queen;
    if (nameStr == "King")
        return Name::King;
    if (nameStr == "Pawn")
        return Name::Pawn;
    throw std::invalid_argument("Nom de pièce invalide !");
}

// Convertir une string en Color
Piece::Color Piece::stringToColor(const std::string& colorStr)
{
    if (colorStr == "White")
        return Color::White;
    if (colorStr == "Black")
        return Color::Black;
    throw std::invalid_argument("Couleur invalide !");
}

Piece::State Piece::get_state() const
{
    return state;
}

std::pair<int, int> Piece::get_position() const
{
    return position;
}

std::string Piece::get_type() const
{
    std::string nameStr;
    switch (name)
    {
    case Name::Rook: nameStr = "Rook"; break;
    case Name::Knight: nameStr = "Knight"; break;
    case Name::Bishop: nameStr = "Bishop"; break;
    case Name::Queen: nameStr = "Queen"; break;
    case Name::King: nameStr = "King"; break;
    case Name::Pawn: nameStr = "Pawn"; break;
    }
    return (color == Color::White ? "White " : "Black ") + nameStr;
}

std::vector<std::pair<int, int>> Piece::possible_moves() const
{
    std::vector<std::pair<int, int>> possible_moves;
    int                              x = position.first;
    int                              y = position.second;

    switch (name)
    {
    case Name::Pawn:
        if (color == Color::White)
        {
            // Pion blanc : peut avancer de 1 ou 2 cases si en position initiale
            if (y == 1 && y + 2 < 8)
            {
                possible_moves.push_back({x, y + 2});
            }
            if (y + 1 < 8)
            {
                possible_moves.push_back({x, y + 1});
            }
        }
        else
        { // Pion noir
            // Pion noir : peut avancer de 1 ou 2 cases si en position initiale
            if (y == 6 && y - 2 >= 0)
            {
                possible_moves.push_back({x, y - 2});
            }
            if (y - 1 >= 0)
            {
                possible_moves.push_back({x, y - 1});
            }
        }
        break;
    
    case Name::Bishop:
        for (int i = 1; i < 8; i++) {
            if (x - i >= 0 && y + i < 8) possible_moves.push_back({x - i, y + i});
            if (x - i >= 0 && y - i >= 0) possible_moves.push_back({x - i, y - i});
            if (x + i < 8 && y + i < 8) possible_moves.push_back({x + i, y + i});
            if (x + i < 8 && y - i >= 0) possible_moves.push_back({x + i, y - i});
        }
        break;

    case Name::Knight:
        for (const auto& [dx, dy] : std::vector<std::pair<int, int>>{
                 {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
             })
        {
            int newX = x + dx, newY = y + dy;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                possible_moves.push_back({newX, newY});
        }
        break;

    case Name::Rook:
    {
        // Vers la droite
        for (int i = x + 1; i < 8; i++)
        {
            possible_moves.push_back({i, y});
        }

        // Vers la gauche
        for (int i = x - 1; i >= 0; i--)
        {
            possible_moves.push_back({i, y});
        }

        // Vers le haut
        for (int i = y + 1; i < 8; i++)
        {
            possible_moves.push_back({x, i});
        }

        // Vers le bas
        for (int i = y - 1; i >= 0; i--)
        {
            possible_moves.push_back({x, i});
        }
        break;
    }

    case Name::Queen:
    {
        // Vers la droite
        for (int i = x + 1; i < 8; i++)
        {
            possible_moves.push_back({i, y});
        }

        // Vers la gauche
        for (int i = x - 1; i >= 0; i--)
        {
            possible_moves.push_back({i, y});
        }

        // Vers le haut
        for (int i = y + 1; i < 8; i++)
        {
            possible_moves.push_back({x, i});
        }

        // Vers le bas
        for (int i = y - 1; i >= 0; i--)
        {
            possible_moves.push_back({x, i});
        }

        // Diagonale haut-droite
        for (int i = 1; x + i < 8 && y + i < 8; i++)
        {
            possible_moves.push_back({x + i, y + i});
        }

        // Diagonale bas-droite
        for (int i = 1; x + i < 8 && y - i >= 0; i++)
        {
            possible_moves.push_back({x + i, y - i});
        }

        // Diagonale haut-gauche
        for (int i = 1; x - i >= 0 && y + i < 8; i++)
        {
            possible_moves.push_back({x - i, y + i});
        }

        // Diagonale bas-gauche
        for (int i = 1; x - i >= 0 && y - i >= 0; i++)
        {
            possible_moves.push_back({x - i, y - i});
        }
        break;

    case Name::King:
    {
        // Déplacements possibles du roi (une case dans toutes les directions)
        std::vector<std::pair<int, int>> directions = {
            {1, 0},  // Droite
            {-1, 0}, // Gauche
            {0, 1},  // Haut
            {0, -1}, // Bas
            {1, 1},  // Diagonale haut-droite
            {1, -1}, // Diagonale bas-droite
            {-1, 1}, // Diagonale haut-gauche
            {-1, -1} // Diagonale bas-gauche
        };

        for (const auto& [dx, dy] : directions)
        {
            int newX = x + dx;
            int newY = y + dy;

            // Vérifier que la case est bien dans les limites de l'échiquier
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
            {
                possible_moves.push_back({newX, newY});
            }
        }

        break;
    }
    default:
        break;
    }

        return possible_moves;
    }
}
    void Piece::move(const std::pair<int, int>& newPosition)
    {
        std::vector<std::pair<int, int>> moves = possible_moves();

        if (std::find(moves.begin(), moves.end(), newPosition) != moves.end())
        {
            position = newPosition;
        }
        else
        {
            std::cout << "Faux" << std::endl;
        }
    }