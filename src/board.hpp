#pragma once
#include <imgui.h>
#include <iostream>
#include <vector>
#include "piece.hpp"
#include "quick_imgui/quick_imgui.hpp"

class Board {
private:
    std::vector<std::vector<Piece*>> grid;                                         // 8x8 pour les pièces
    ImVec2                           board_pos      = ImVec2(50, 50);              // Position de l'interface du plateau
    float                            square_size    = 50.0f;                       // Taille d'une case en pixels
    bool                             selected_piece = false;                       // Pour savoir si une pièce est sélectionnée
    std::pair<int, int>              selected_pos;                                 // Position de la pièce sélectionnée
    Piece::Color                     last_moved_piece_color = Piece::Color::Black; // Couleur de la dernière pièce bougé

public:
    Board();
    ~Board();

    // Afficher le plateau avec IMGui
    void render();

    // Placer une pièce sur le plateau
    void placePiece(Piece* piece, int x, int y);

    // Déplacer une pièce avec IMGui
    bool movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to);

    // Gérer la sélection d'une pièce
    void handle_piece_selection(const std::pair<int, int>& pos);

    // Récupérer une pièce à une position donnée
    Piece* get_piece_at(const std::pair<int, int>& pos);

    // Vérifier si un mouvement est valide
    bool is_valid_move(const std::pair<int, int>& from, const std::pair<int, int>& to);

    // Informe si une pièce est présente sur une case donnée
    bool is_piece_at(const std::pair<int, int>& pos);

    bool is_game_over();

    // Fonction pour obtenir le label du bouton d'une pièce (ex. "PW", "PN", "RW", "RN", etc.)
    std::string getPieceButtonLabel(const std::string& piece_type, const std::string& color);
};
