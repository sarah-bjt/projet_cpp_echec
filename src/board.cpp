#include "board.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"

void Board_sarah::draw_pion_line()
{
    for (int i{0}; i < 8; i++)
    {
        ImGui::Button("Pion", ImVec2{50.f, 50.f});
        ImGui::SameLine();
    }
}

void Board_sarah::draw_vide_line()
{
    for (int i{0}; i < 8; i++)
    {
        ImGui::Button("_", ImVec2{50.f, 50.f});
        ImGui::SameLine();
    }
}

void Board_sarah::draw_royal_line()
{
    ImGui::Button("tour", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("cavalier", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("fou", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("reine", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("roi", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("fou", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("cavalier", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("tour", ImVec2{50.f, 50.f});
    ImGui::SameLine();
}

void Board_sarah::draw_board()
{
    Board_sarah::draw_royal_line();
    ImGui::NewLine();
    Board_sarah::draw_pion_line();
    ImGui::NewLine();
    Board_sarah::draw_vide_line();
    ImGui::NewLine();
    Board_sarah::draw_vide_line();
    ImGui::NewLine();
    Board_sarah::draw_vide_line();
    ImGui::NewLine();
    Board_sarah::draw_pion_line();
    ImGui::NewLine();
    Board_sarah::draw_royal_line();
    ImGui::NewLine();
}