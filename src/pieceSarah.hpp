#pragma once
#include <imgui.h>
#include <string>
#include <utility>

class piece {
    enum class type : std::uint8_t {
        pawn,
        bishop
    };
    std::pair<int, int> position;
    enum class etat : std::uint8_t {
        vivant,
        mort,
        malade
    };
};
