#include <iostream>
#include "boardGwat.hpp"
#include <imgui.h>

void BoardGwat::CreateBoard(int NumberBox) {

    std::vector<std::string> ChessPiece = {"Rook 1", "Knight 1", "Bishop 1", "Queen", "King", "Bishop 2", "Knight 2", "Rook 2", "Pawn"};
    
    for (int row = 0; row < NumberBox; row++) {
        for (int col = 0; col < NumberBox; col++) {

            int i = row * 8 + col;
            ImGui::PushID(i);

            if (col == 0 && row > 0) {
                ImGui::NewLine();
            }
            if(row == 0 || row == 7) {
                ImGui::SameLine();
                ImGui::Button(ChessPiece[col].c_str(), ImVec2{50.0, 50.0});
            }
            else if(row == 1 || row == 6) {
                ImGui::SameLine();
                ImGui::Button(ChessPiece[8].c_str(), ImVec2{50.0, 50.0});
            }
            else {
                ImGui::SameLine();
                ImGui::Button("##id", ImVec2{50.0, 50.0});
            }
            ImGui::PopID();
        }   
    }
}