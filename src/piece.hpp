#pragma once
#include <iostream>
#include <vector>

class Piece {
private:
    std::pair<std::string, int> type;
    enum class State {
        Dead,
        Alive,
    };
    State state;
    enum class Name {
        Rook,
        Knight,
        Bishop,
        Queen,
        King,
        Pawn
    };
    Name                name;
    std::pair<int, int> position;

public:
    std::vector<std::pair<int, int>> possible_moves() const;                        // Retourne les déplacements possibles
    void                             move(const std::pair<int, int>& newPosition);  // Déplace la pièce
    std::vector<std::pair<int, int>> possible_moves(const std::string& type) const; // Retourne les déplacements possibles
    std::pair<int, int>              get_position() const;                          // Récupère la position actuelle
    std::string                      get_type() const;                              // Récupère le type de la pièce
    State                            get_state() const;                             // Récupère l'état de la pièce
    void                             set_state(State newState);                     // Modifie l'état de la pièce

    // constructor
    Piece(const std::string& name, int color, const std::pair<int, int>& startPosition);
};