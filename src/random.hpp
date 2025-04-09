#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <string>
#include <utility> // pour std::pair
#include <vector>
#include "maths.hpp" // Pour accéder à globalRandom

// Forward declaration
class Board;
class Piece;

class ChessRandomizer {
public:
    /**
     * Randomise la promotion d'un pion
     * @param pieceName Référence vers la variable qui stockera le nom de la pièce
     * @param pieceColor Référence vers la variable qui stockera la couleur
     * @return true si la promotion a été randomisée
     */
    static bool randomizePromotion(std::string& pieceName, bool& changeColor);

    /**
     * Randomise les mouvements possibles d'une pièce
     * @param moves Référence vers le vecteur de mouvements possibles (sera modifié)
     * @param board Plateau de jeu pour vérifier la validité des mouvements
     * @return true si les mouvements ont été randomisés
     */
    static bool randomizeMovements(std::vector<std::pair<int, int>>& moves, const std::vector<std::vector<Piece*>>& grid);

    /**
     * Randomise la position d'une pièce sur le plateau
     * @param x Référence vers la coordonnée x (sera modifiée)
     * @param y Référence vers la coordonnée y (sera modifiée)
     * @param grid La grille du plateau pour vérifier si la case est libre
     * @return true si la position a été randomisée
     */
    static bool randomizePosition(int& x, int& y, const std::vector<std::vector<Piece*>>& grid);

    /**
     * Détermine aléatoirement si un événement doit se produire
     * @param probability Probabilité que l'événement se produise (0.0 - 1.0)
     * @return true si l'événement doit se produire
     */
    static bool shouldHappen(double probability);
};

#endif // RANDOM_HPP