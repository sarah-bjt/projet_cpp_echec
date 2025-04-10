#include "piece.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

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

void Piece::set_state(State newState)
{
    this->state = newState;
}

void Piece::promote(std::string newType, bool random)
{
    if (newType == "Queen")
        name = Name::Queen;
    else if (newType == "Rook")
        name = Name::Rook;
    else if (newType == "Bishop")
        name = Name::Bishop;
    else if (newType == "Knight")
        name = Name::Knight;
    if (random)
    {
        name  = static_cast<Name>(globalRandom.uniformDiscrete(0, 5));
        color = (globalRandom.bernoulli(0.7)) ? color : (color == Color::White ? Color::Black : Color::White);
    }
}

Piece::Color Piece::get_color() const
{
    return color;
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

std::string Piece::get_name() const
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
    return nameStr;
}

// Vérifier si une case contient une pièce adverse
bool Piece::is_enemy(const std::pair<int, int>& pos, const std::vector<std::vector<Piece*>>& board) const
{
    Piece* target = board[pos.first][pos.second];
    return target && target->color != color;
}

// Vérifier si une case est vide
bool Piece::is_empty(const std::pair<int, int>& pos, const std::vector<std::vector<Piece*>>& board) const
{
    return board[pos.first][pos.second] == nullptr;
}

// Générer les mouvements possibles
std::vector<std::pair<int, int>> Piece::possible_moves(const std::vector<std::vector<Piece*>>& board) const
{
    std::vector<std::pair<int, int>> possible_moves;
    int                              x = position.first, y = position.second;

    switch (name)
    {
    case Name::Pawn:
    {
        if (color == Color::White)
        {
            // Déplacement d'une case en avant
            if (y + 1 < 8 && is_empty({x, y + 1}, board))
                possible_moves.push_back({x, y + 1});

            // Mouvement de deux cases (si c'est le premier mouvement du pion)
            if (y == 1 && is_empty({x, y + 2}, board) && is_empty({x, y + 1}, board))
                possible_moves.push_back({x, y + 2});

            // Capture en diagonale (vers la droite et la gauche)
            if (x - 1 >= 0 && y + 1 < 8 && is_enemy({x - 1, y + 1}, board))
                possible_moves.push_back({x - 1, y + 1});
            if (x + 1 < 8 && y + 1 < 8 && is_enemy({x + 1, y + 1}, board))
                possible_moves.push_back({x + 1, y + 1});

            // Prise en passant (si un pion adverse a bougé de deux cases) - Pas terminer il manque la disparition de la piece adverse
            // if (y == 4)
            // {
            //     // Vérification des pions adverses qui se trouvent sur la même ligne
            //     if (x - 1 >= 0 && is_enemy({x - 1, y}, board) && is_empty({x - 1, y + 1}, board))
            //     {
            //         possible_moves.push_back({x - 1, y + 1});
            //     }
            //     if (x + 1 < 8 && is_enemy({x + 1, y}, board) && is_empty({x + 1, y + 1}, board))
            //     {
            //         possible_moves.push_back({x + 1, y + 1});
            //     }
            // }
        }
        else
        {
            // Déplacement d'une case en avant
            if (y - 1 >= 0 && is_empty({x, y - 1}, board))
                possible_moves.push_back({x, y - 1});

            // Mouvement de deux cases (si c'est le premier mouvement du pion)
            if (y == 6 && is_empty({x, y - 2}, board) && is_empty({x, y - 1}, board))
                possible_moves.push_back({x, y - 2});

            // Capture en diagonale (vers la droite et la gauche)
            if (x - 1 >= 0 && y - 1 >= 0 && is_enemy({x - 1, y - 1}, board))
                possible_moves.push_back({x - 1, y - 1});
            if (x + 1 < 8 && y - 1 >= 0 && is_enemy({x + 1, y - 1}, board))
                possible_moves.push_back({x + 1, y - 1});

            // Prise en passant (si un pion adverse a bougé de deux cases) - Pas terminer il manque la disparition de la piece adverse
            // if (y == 3)
            // {
            //     // Vérification des pions adverses qui se trouvent sur la même ligne
            //     if (x - 1 >= 0 && is_enemy({x - 1, y}, board) && is_empty({x - 1, y - 1}, board))
            //     {
            //         possible_moves.push_back({x - 1, y - 1});
            //     }
            //     if (x + 1 < 8 && is_enemy({x + 1, y}, board) && is_empty({x + 1, y - 1}, board))
            //     {
            //         possible_moves.push_back({x + 1, y - 1});
            //     }
            // }
        }
    }
    case Name::Bishop:
    case Name::Rook:
    case Name::Queen:
    {
        std::vector<std::pair<int, int>> directions;
        if (name == Name::Bishop || name == Name::Queen)
            directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        if (name == Name::Rook || name == Name::Queen)
            directions.insert(directions.end(), {{1, 0}, {-1, 0}, {0, 1}, {0, -1}});

        for (const auto& [dx, dy] : directions)
        {
            for (int i = 1; i < 8; i++)
            {
                int newX = x + i * dx, newY = y + i * dy;
                if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
                    break;
                if (is_empty({newX, newY}, board))
                    possible_moves.push_back({newX, newY});
                else
                {
                    if (is_enemy({newX, newY}, board))
                        possible_moves.push_back({newX, newY});
                    break;
                }
            }
        }
        break;
    }

    case Name::Knight:
    {
        for (const auto& [dx, dy] : std::vector<std::pair<int, int>>{
                 {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
             })
        {
            int newX = x + dx, newY = y + dy;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 && (is_empty({newX, newY}, board) || is_enemy({newX, newY}, board)))
                possible_moves.push_back({newX, newY});
        }
        break;
    }

    case Name::King:
    {
        for (const auto& [dx, dy] : std::vector<std::pair<int, int>>{
                 {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
             })
        {
            int newX = x + dx, newY = y + dy;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 && (is_empty({newX, newY}, board) || is_enemy({newX, newY}, board)))
                possible_moves.push_back({newX, newY});
        }
        break;
    }
    }

    return possible_moves;
}

void Piece::move(const std::pair<int, int>& newPosition, std::vector<std::vector<Piece*>>& board)
{
    std::vector<std::pair<int, int>> moves = possible_moves(board);

    if (std::find(moves.begin(), moves.end(), newPosition) != moves.end())
    {
        // Capturer une pièce ennemie si présente
        if (board[newPosition.first][newPosition.second] != nullptr)
        {
            delete board[newPosition.first][newPosition.second]; // Suppression de la pièce capturée
            board[newPosition.first][newPosition.second] = nullptr;
        }

        // Déplacer la pièce sur l'échiquier
        board[position.first][position.second]       = nullptr; // Libérer l'ancienne case
        position                                     = newPosition;
        board[newPosition.first][newPosition.second] = this; // Nouvelle position
    }
}
