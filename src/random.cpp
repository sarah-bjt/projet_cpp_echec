// random.cpp

#include "random.hpp"
#include <iostream>
#include "board.hpp"
#include "piece.hpp"

bool ChessRandomizer::randomizePromotion(std::string& pieceName, bool& changeColor)
{
    // Tableau des types de pièces possibles pour la promotion
    const std::string piece_types[] = {"Queen", "Rook", "Bishop", "Knight"};

    // Sélection aléatoire d'une pièce (using uniformDiscrete from globalRandom)
    int random_piece = globalRandom.uniformDiscrete(0, 3);
    pieceName        = piece_types[random_piece];

    // Probabilité de 30% de changer la couleur de la pièce
    changeColor = globalRandom.bernoulli(0.3);
    if (changeColor)
    {
        std::cout << "Surprise! The promoted piece changed color!" << std::endl;
    }

    std::cout << "Random promotion to: " << pieceName << std::endl;
    return true;
}

// bool ChessRandomizer::randomizeMovements(std::vector<std::pair<int, int>>& moves, const std::vector<std::vector<Piece*>>& grid)
// {
//     // Aucun mouvement à randomiser
//     if (moves.empty())
//     {
//         return false;
//     }

//     // Probabilité de 20% de retirer un mouvement au hasard
//     if (globalRandom.bernoulli(0.2) && moves.size() > 1)
//     {
//         int index_to_remove = globalRandom.uniformDiscrete(0, moves.size() - 1);
//         moves.erase(moves.begin() + index_to_remove);
//         std::cout << "A possible move mysteriously disappeared!" << std::endl;
//     }

//     // Probabilité de 10% d'ajouter un mouvement aléatoire (mais valide sur l'échiquier)
//     if (globalRandom.bernoulli(0.1))
//     {
//         int                 new_x = globalRandom.uniformDiscrete(0, 7);
//         int                 new_y = globalRandom.uniformDiscrete(0, 7);
//         std::pair<int, int> new_move(new_x, new_y);

//         // Vérifier que ce mouvement n'existe pas déjà
//         bool exists = false;
//         for (const auto& move : moves)
//         {
//             if (move.first == new_move.first && move.second == new_move.second)
//             {
//                 exists = true;
//                 break;
//             }
//         }

//         if (!exists && (grid[new_x][new_y] == nullptr || grid[new_x][new_y]->get_color() != grid[moves[0].first][moves[0].second]->get_color()))
//         {
//             // Ajouter uniquement si la case est vide ou contient une pièce adverse
//             moves.push_back(new_move);
//             std::cout << "A magical new move appeared at ("
//                       << new_move.first << ", " << new_move.second << ")!" << std::endl;
//         }
//     }

//     return true;
// }

// bool ChessRandomizer::randomizePosition(int& x, int& y, const std::vector<std::vector<Piece*>>& grid)
// {
//     // Probabilité de 5% de téléporter la pièce ailleurs
//     if (globalRandom.bernoulli(0.05))
//     {
//         // Sauvegarder les anciennes coordonnées
//         int old_x = x;
//         int old_y = y;

//         // Tenter jusqu'à 10 fois de trouver une case valide
//         for (int attempt = 0; attempt < 10; attempt++)
//         {
//             // Générer de nouvelles coordonnées
//             int new_x = globalRandom.uniformDiscrete(0, 7);
//             int new_y = globalRandom.uniformDiscrete(0, 7);

//             // Si la case est vide et différente de la position actuelle
//             if ((new_x != old_x || new_y != old_y) && grid[new_x][new_y] == nullptr)
//             {
//                 x = new_x;
//                 y = new_y;
//                 std::cout << "Teleportation from (" << old_x << ", " << old_y
//                           << ") to (" << x << ", " << y << ")!" << std::endl;
//                 return true;
//             }
//         }
//     }

//     return false;
// }

bool ChessRandomizer::shouldHappen(double probability)
{
    return globalRandom.bernoulli(probability);
}