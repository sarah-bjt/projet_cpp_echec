// le gentil board.hpp

// board.hpp

#pragma once
#include <imgui.h>
#include <iostream>
#include <vector>
#include "piece.hpp"
#include "quick_imgui/quick_imgui.hpp"

class Board {
private:
    std::vector<std::vector<Piece*>> grid{8, std::vector<Piece*>(8, nullptr)};
    bool                             activate_random = false;

    double randomColorR;
    double randomColorV;
    double randomColorB;

    // std::vector<std::vector<Piece*>> grid;                         // 8x8 pour les pièces
    ImVec2              board_pos   = ImVec2(50, 50); // Position de l'interface du plateau
    float               square_size = 60.0f;          // Taille d'une case en pixels
    ImU32               squareColor;
    bool                selected_piece         = false;               // Pour savoir si une pièce est sélectionnée
    std::pair<int, int> selected_pos           = {-1, -1};            // Position de la pièce sélectionnée
    Piece::Color        last_moved_piece_color = Piece::Color::Black; // Couleur de la dernière pièce bougé, initialisé à noir

    // polices
    ImFont *font1, *font2, *font3, *font4, *font5, *font6, *font7, *font8, randomFont;
    int     chosenFontIndex; // Pour stocker l'indice de la police choisie
    ImFont* chosenFont;      // Pour stocker la police choisie

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
    void handleRandom();
    void promotePawn(const std::string& pieceType, bool aleat);

    // Méthodes de logique de jeu
    bool isValidMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to);
    void capturePieceIfPresent(const std::pair<int, int>& position);
    void performMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to);
    bool checkForPawnPromotion(Piece* piece, const std::pair<int, int>& position);

public:
    Board() = default;
    ~Board();

    void init();
    void reset();

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

    std::pair<bool, int> is_game_over();

    std::string getPieceButtonLabel(const std::string& piece_type, const std::string& color);
};