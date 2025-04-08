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
    Piece::Color                     last_moved_piece_color = Piece::Color::Black; // Couleur de la dernière pièce bougé, initialisé à noir

    // pour faire la promo d'un pion
    bool                promotion_active = false; // Indique si la promotion est en cours
    std::pair<int, int> promotion_pos;            // Position du pion à promouvoir
    Piece*              promoted_piece = nullptr; // Stocke le pion qui doit être promu

    // Méthodes d'affichage
    void renderBoardSquares();
    void renderPieces();
    void renderPieceAt(Piece* piece, int x, int y);
    void renderPossibleMoves();
    void renderMoveIndicator(Piece* piece, const std::pair<int, int>& move);
    void renderSelectionInfo();

    // Méthodes de gestion d'interaction
    void handlePieceSelection(int x, int y);
    void handleMoveSelection(const std::pair<int, int>& move);
    void handlePromotionPopup();
    void handleGameOverPopup();
    void promotePawn(const std::string& pieceType, bool aleat);

    // Méthodes de logique de jeu
    bool isValidMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to);
    void capturePieceIfPresent(const std::pair<int, int>& position);
    void performMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to);
    bool checkForPawnPromotion(Piece* piece, const std::pair<int, int>& position);

public:
    Board();
    ~Board();

    void init();

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