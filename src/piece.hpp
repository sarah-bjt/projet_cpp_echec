#pragma once
#include <iostream>
#include <vector>

class Piece {
public:
    enum class State : uint8_t {
        Dead,
        Alive,
    };

    enum class Color : uint8_t {
        Black,
        White
    };

private:
    enum class Name : uint8_t {
        Rook,
        Knight,
        Bishop,
        Queen,
        King,
        Pawn
    };

    State               state;
    Color               color;
    Name                name;
    std::pair<int, int> position;

    // Convertir std::string en enum
    static Name  stringToName(const std::string& nameStr);
    static Color stringToColor(const std::string& colorStr);

public:
    Piece(const std::string& name, const std::string& color, const std::pair<int, int>& startPosition);
    std::vector<std::pair<int, int>> possible_moves(const std::vector<std::vector<Piece*>>& board) const;
    void                             move(const std::pair<int, int>& newPosition, std::vector<std::vector<Piece*>>& board);
    std::pair<int, int>              get_position() const;
    std::string                      get_type() const;
    State                            get_state() const;
    Color                            get_color() const;
    std::string                      get_name() const;
    void                             set_state(State newState);
    bool                             is_enemy(const std::pair<int, int>& pos, const std::vector<std::vector<Piece*>>& board) const;
    bool                             is_empty(const std::pair<int, int>& pos, const std::vector<std::vector<Piece*>>& board) const;
    void                             promote(std::string newType);
};