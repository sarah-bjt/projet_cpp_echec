#include "board.hpp"
#include <imgui.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "piece.hpp"

// Surcharge de l'opérateur + pour ImVec2
ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

// Constructeur pour initialiser un plateau vide et placer les pièces
Board::Board()
{
    grid.resize(8, std::vector<Piece*>(8, nullptr));

    // Placer les pièces blanches (en bas sur la ligne 0)
    placePiece(new Piece("Rook", "White", {0, 0}), 0, 0);
    placePiece(new Piece("Rook", "White", {7, 0}), 7, 0);
    placePiece(new Piece("Knight", "White", {1, 0}), 1, 0);
    placePiece(new Piece("Knight", "White", {6, 0}), 6, 0);
    placePiece(new Piece("Bishop", "White", {2, 0}), 2, 0);
    placePiece(new Piece("Bishop", "White", {5, 0}), 5, 0);
    placePiece(new Piece("Queen", "White", {3, 0}), 3, 0);
    placePiece(new Piece("King", "White", {4, 0}), 4, 0);

    // Placer les pions blancs (en ligne 1)
    for (int x = 0; x < 8; ++x)
    {
        placePiece(new Piece("Pawn", "White", {x, 1}), x, 1);
    }

    // Placer les pièces noires (en haut sur la ligne 7)
    placePiece(new Piece("Rook", "Black", {0, 7}), 0, 7);
    placePiece(new Piece("Rook", "Black", {7, 7}), 7, 7);
    placePiece(new Piece("Knight", "Black", {1, 7}), 1, 7);
    placePiece(new Piece("Knight", "Black", {6, 7}), 6, 7);
    placePiece(new Piece("Bishop", "Black", {2, 7}), 2, 7);
    placePiece(new Piece("Bishop", "Black", {5, 7}), 5, 7);
    placePiece(new Piece("Queen", "Black", {3, 7}), 3, 7);
    placePiece(new Piece("King", "Black", {4, 7}), 4, 7);

    // Placer les pions noirs (en ligne 6)
    for (int x = 0; x < 8; ++x)
    {
        placePiece(new Piece("Pawn", "Black", {x, 6}), x, 6);
    }
}

// Méthode d'initialisation du plateau
void Board::init()
{
    // Initialiser la position du plateau sur l'écran
    board_pos = ImVec2(50, 50);  // Position du coin supérieur gauche du plateau
    
    // Définir la taille des cases
    square_size = 60;  // Taille en pixels de chaque case
    
    // Initialiser les variables de sélection
    selected_piece = false;
    selected_pos = {-1, -1};
    
    // Initialiser les variables de promotion
    promotion_active = false;
    promoted_piece = nullptr;
    
    // Initialiser la couleur de la dernière pièce déplacée
    // Par défaut, commencer avec les noirs pour permettre aux blancs de jouer en premier
    last_moved_piece_color = Piece::Color::Black;
    
    // Autres initialisations éventuelles
    std::cout << "Plateau d'échecs initialisé" << std::endl;
}

// Destructeur pour libérer la mémoire des pièces
Board::~Board()
{
    for (auto& row : grid)
    {
        for (auto& piece : row)
        {
            delete piece;
        }
    }
}

// Placer une pièce sur une case donnée
void Board::placePiece(Piece* piece, int x, int y)
{
    grid[x][y] = piece;
}

bool Board::is_game_over()
{
    bool whiteKingAlive = false;
    bool blackKingAlive = false;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = grid[x][y];
            if (piece != nullptr)
            {
                if (piece->get_type() == "White King" || piece->get_type() == "Black King")
                {
                    if (piece->get_color() == Piece::Color::White && piece->get_state() == Piece::State::Alive)
                    {
                        whiteKingAlive = true;
                    }
                    else if (piece->get_color() == Piece::Color::Black && piece->get_state() == Piece::State::Alive)
                    {
                        blackKingAlive = true;
                    }
                }
            }
        }
    }

    // Si l'un des rois est mort, la partie est finie
    if (!whiteKingAlive)
    {
        std::cout << "Le roi blanc a été capturé. La partie est terminée." << std::endl;
        return true; // La partie est terminée
    }
    if (!blackKingAlive)
    {
        std::cout << "Le roi noir a été capturé. La partie est terminée." << std::endl;
        return true; // La partie est terminée
    }

    return false;
}

// Afficher le plateau
void Board::render()
{
    bool piece_clicked = false;

    // 1. Dessiner les cases
    for (int y = 7; y >= 0; --y)
    {
        for (int x = 0; x < 8; ++x)
        {
            ImVec2 pos  = board_pos + ImVec2(x * square_size, (7 - y) * square_size); // Inverser y pour que (0,0) soit en bas à gauche
            ImVec2 size = ImVec2(square_size, square_size);

            // Dessiner les cases
            ImU32 color = (x + y) % 2 == 0 ? IM_COL32(119, 148, 85, 255) : IM_COL32(240, 217, 181, 255);
            ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + square_size, pos.y + square_size), color);
        }
    }

    // 2. Afficher les pièces après les cases
    for (int y = 7; y >= 0; --y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = grid[x][y];
            if (piece != nullptr)
            {
                ImVec2      pos       = board_pos + ImVec2(x * square_size, (7 - y) * square_size); // Inverser y pour que (0,0) soit en bas à gauche
                std::string piece_str = piece->get_name();

                // Dessiner la pièce
                ImGui::SetCursorScreenPos(pos);
                std::string button_label = piece_str + "##" + std::to_string(x) + std::to_string(y);

                ImGui::PushStyleColor(ImGuiCol_Button, piece->get_color() == Piece::Color::White ? IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255));
                ImGui::PushStyleColor(ImGuiCol_Text, piece->get_color() == Piece::Color::White ? IM_COL32(0, 0, 0, 255) : IM_COL32(255, 255, 255, 255));

                if (ImGui::Button(button_label.c_str(), ImVec2(square_size, square_size)))
                {
                    if (selected_piece && selected_pos.first == x && selected_pos.second == y)
                    {
                        // Désélectionner la pièce si on clique dessus une deuxième fois
                        selected_piece = false;
                        // std::cout << "Désélection de la pièce: (" << x << ", " << y << ")\n";
                    }
                    else
                    {
                        // Sélectionner la nouvelle pièce
                        selected_piece = true;
                        selected_pos   = {x, y};
                        // std::cout << "Sélection de la pièce: (" << x << ", " << y << ")\n";
                    }
                }

                ImGui::PopStyleColor(2);
            }
        }
    }

    // 3. Afficher les mouvements possibles de la pièce sélectionnée
    if (selected_piece)
    {
        Piece* piece = grid[selected_pos.first][selected_pos.second];
        if (piece != nullptr)
        {
            // Obtenir les mouvements possibles de la pièce
            std::vector<std::pair<int, int>> moves = piece->possible_moves(grid);

            std::cout << "Mouvements possibles pour la pièce sélectionnée: (" << selected_pos.first << ", " << selected_pos.second << ")\n";

            for (const auto& move : moves)
            {
                // Calculer la position du cercle de mouvement (en tenant compte de l'inversion de y)
                ImVec2 move_pos = board_pos + ImVec2(move.first * square_size, (7 - move.second) * square_size);

                // Vérifier si une pièce est déjà présente sur la case
                if (!is_piece_at(move)) // Si la case n'est pas occupée par une pièce
                {
                    if (piece->get_color() == last_moved_piece_color)
                    {
                        // Dessiner un cercle vert transparent à la position du mouvement possible mais pas jouble
                        ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(move_pos.x + square_size / 2, move_pos.y + square_size / 2), 10.0f, IM_COL32(0, 100, 0, 100));
                    }
                    else
                    {
                        // Dessiner un cercle vert à la position du mouvement possible
                        ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(move_pos.x + square_size / 2, move_pos.y + square_size / 2), 10.0f, IM_COL32(0, 255, 0, 255));
                    }
                }

                // Gérer le clic pour déplacer la pièce
                if (ImGui::IsMouseHoveringRect(move_pos, ImVec2(move_pos.x + square_size, move_pos.y + square_size)) && ImGui::IsMouseClicked(0))
                {
                    std::cout << "Déplacement effectué vers (" << move.first << ", " << move.second << ")\n";
                    movePiece(selected_pos, move);
                    selected_piece = false; // Désélectionner après déplacement
                }
            }
        }
    }

    // 4. Afficher un message en bas indiquant la case sélectionnée
    if (selected_piece)
    {
        std::string msg = "Case sélectionnée : (" + std::to_string(selected_pos.first) + ", " + std::to_string(selected_pos.second) + ")";
        ImGui::Text("%s", msg.c_str());
    }

    if (promotion_active)
    {
        ImGui::OpenPopup("Promotion");
    }

    if (ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Choisissez une promotion:");

        if (ImGui::Button("Queen"))
        {
            promoted_piece->promote("Queen");
            promotion_active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Rook"))
        {
            promoted_piece->promote("Rook");
            promotion_active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Bishop"))
        {
            promoted_piece->promote("Bishop");
            promotion_active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Knight"))
        {
            promoted_piece->promote("Knight");
            promotion_active = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (is_game_over())
    {
        ImGui::OpenPopup("Game Over");
    }

    if (ImGui::BeginPopupModal("Game Over", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Le roi a été capturé ! La partie est terminée.");

        if (ImGui::Button("Quitter"))
        {
            exit(0); // Ferme le programme
        }

        ImGui::EndPopup();
    }
}

// Fonction pour déplacer une pièce d'une case à une autre
bool Board::movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to)
{
    Piece* piece = grid[from.first][from.second];
    if (piece == nullptr)
        return false;

    Piece* target_piece = grid[to.first][to.second];
    if (target_piece != nullptr)
    {
        // La pièce cible existe et doit être capturée
        target_piece->set_state(Piece::State::Dead);
        // std::cout << "Capture de la pièce à (" << to.first << ", " << to.second << ")\n";
    }

    // Vérifier si le mouvement est valide via la méthode move de la pièce
    std::vector<std::pair<int, int>> moves = piece->possible_moves(grid);
    if (std::find(moves.begin(), moves.end(), to) == moves.end() || piece->get_color() == last_moved_piece_color)
    {
        // std::cout << "Mouvement invalide pour " << piece->get_type()
        //           << " de (" << from.first << "," << from.second << ")"
        //           << " à (" << to.first << "," << to.second << ")" << std::endl;
        return false; // Mouvement invalide
    }

    // Si le mouvement est valide, procéder au déplacement
    piece->move(to, grid);                   // Effectuer le mouvement dans la pièce
    grid[to.first][to.second]     = piece;   // Mettre à jour la grille
    grid[from.first][from.second] = nullptr; // Libérer la case d'origine
    // Met à jour la dernière couleur déplacée
    last_moved_piece_color = piece->get_color();

    if ((piece->get_type() == "White Pawn" || piece->get_type() == "Black Pawn") && (to.second == 0 || to.second == 7))
    {
        std::cout << "Le pion atteint la dernière ligne et peut être promu !" << std::endl;
        // Activer la promotion
        promotion_active = true;
        promotion_pos    = to;
        promoted_piece   = piece;
        ImGui::OpenPopup("Promotion");
        return true; // Ne pas continuer tant que la promotion n'est pas choisie
    }

    return true; // Mouvement valide, on a effectué le déplacement
}

// Informe si une pièce est présente sur une case donnée
bool Board::is_piece_at(const std::pair<int, int>& pos)
{
    return grid[pos.first][pos.second] != nullptr;
}