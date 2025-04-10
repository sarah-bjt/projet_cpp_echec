#pragma once
#include <imgui.h>
#include <iostream>
#include <map>
#include <vector>
#include "piece.hpp"
#include "quick_imgui/quick_imgui.hpp"

class Board {
private:
    // utilisation du random
    double                           randomColorR;
    double                           randomColorV;
    double                           randomColorB;
    ImU32                            squareColor;
    ImU32                            dotColor_light, dotColor_dark;
    std::vector<std::pair<int, int>> visibleStickyTiles; // Pour les cases collantes visibles
    std::vector<std::pair<int, int>> stickingTiles;      // Pour les cases collantes
    std::vector<float>               stikySizes;         // Pour les tailles des cases collantes

    // initialisation du plateau
    std::vector<std::vector<Piece*>> grid{8, std::vector<Piece*>(8, nullptr)}; // Grille de 8x8 pour le plateau d'échecs
    ImVec2                           board_pos              = ImVec2(0, 0);
    float                            square_size            = 50.0f;
    bool                             selected_piece         = false;               // Pour savoir si une pièce est sélectionnée
    std::pair<int, int>              selected_pos           = {-1, -1};            // Position de la pièce sélectionnée
    Piece::Color                     last_moved_piece_color = Piece::Color::Black; // Couleur de la dernière pièce bougé, initialisé à noir

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
    void renderStickyStuff(auto visiblyStickyTiles);

    // Méthodes de gestion d'interaction
    void handlePieceSelection(int x, int y);
    void handleMoveSelection(const std::pair<int, int>& move);
    void handlePromotionPopup();
    void handleGameOverPopup();
    void handleRandom();
    void promotePawn(const std::string& pieceType, bool aleat);

    // Méthodes de logique de jeu
    std::pair<bool, int> is_game_over();
    bool                 isValidMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to);
    void                 capturePieceIfPresent(const std::pair<int, int>& position);
    void                 performMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to);
    bool                 checkForPawnPromotion(Piece* piece, const std::pair<int, int>& position);

public:
    Board() = default;
    ~Board();
    bool                             activate_random = false;
    std::vector<std::pair<int, int>> stickyTiles();
    std::vector<std::pair<int, int>> visiblySticky(std::vector<std::pair<int, int>> tiles);
    std::vector<float>               roundSize(auto visibleTiles);
    void                             init();
    void                             render();
    void                             placePiece(Piece* piece, int x, int y);
    bool                             movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to);
    Piece*                           get_piece_at(const std::pair<int, int>& pos);
    bool                             is_valid_move(const std::pair<int, int>& from, const std::pair<int, int>& to);
    bool                             is_piece_at(const std::pair<int, int>& pos);
    std::string                      getPieceButtonLabel(const std::string& piece_type, const std::string& color);
};
