#pragma once
#include <iostream>
#include <vector>

class Piece {
    public:
        enum class State : uint8_t {
            Dead,
            Alive,
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
    
        enum class Color : uint8_t {
            Black,
            White
        };
    
        State state;
        Color color;
        Name name;
        std::pair<int, int> position;
    
        // Convertir std::string en enum
        static Name stringToName(const std::string& nameStr);
        static Color stringToColor(const std::string& colorStr);
    
    public:
        Piece(const std::string& name, const std::string& color, const std::pair<int, int>& startPosition);
        std::vector<std::pair<int, int>> possible_moves() const;
        void move(const std::pair<int, int>& newPosition);
        std::pair<int, int> get_position() const;
        std::string get_type() const;
        State get_state() const;
        void set_state(State newState);
    };