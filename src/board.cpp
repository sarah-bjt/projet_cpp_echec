#include <imgui.h>
#include <board.hpp>
#include <string>
#include <vector>
#include "maths.hpp"

namespace {
ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
} // namespace

// Destructeur
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

// Méthode d'initialisation du plateau
void Board::init()
{
    // charge des fonts
    //  Récupérer l'IO d'ImGui
    ImGuiIO& io = ImGui::GetIO();
    // Charger différentes polices
    float fontSize = 17.0f;
    font1          = io.Fonts->AddFontDefault();
    font2          = io.Fonts->AddFontFromFileTTF("../../import/font/DroidSans.ttf", fontSize);
    font3          = io.Fonts->AddFontFromFileTTF("../../import/font/Cousine-Regular.ttf", fontSize);
    font4          = io.Fonts->AddFontFromFileTTF("../../import/font/ReggaeOne-Regular.ttf", fontSize);
    font5          = io.Fonts->AddFontFromFileTTF("../../import/font/ProggyClean.ttf", fontSize);
    font6          = io.Fonts->AddFontFromFileTTF("../../import/font/Karla-Regular.ttf", fontSize);
    font7          = io.Fonts->AddFontFromFileTTF("../../import/font/ProggyTiny.ttf", fontSize);
    font8          = io.Fonts->AddFontFromFileTTF("../../import/font/Roboto-Medium.ttf", fontSize);
    io.Fonts->Build();

    // Choisir une police aléatoire une seule fois
    chosenFontIndex = globalRandom.uniformDiscrete(1, 8);

    // Assigner la police choisie
    switch (chosenFontIndex)
    {
    case 1: chosenFont = font1; break;
    case 2: chosenFont = font2; break;
    case 3: chosenFont = font3; break;
    case 4: chosenFont = font4; break;
    case 5: chosenFont = font5; break;
    case 6: chosenFont = font6; break;
    case 7: chosenFont = font7; break;
    case 8: chosenFont = font8; break;
    default: chosenFont = font1; // Police par défaut
    }

    std::cout << "Police choisie : " << chosenFontIndex << std::endl;

    // Définir une couleur aléatoire pour les cases
    double randomColorR = (globalRandom.uniformContinuous(50, 200));
    double randomColorV = (globalRandom.uniformContinuous(50, 200));
    double randomColorB = (globalRandom.uniformContinuous(50, 200));
    squareColor         = IM_COL32(randomColorR, randomColorV, randomColorB, 255);

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

// Placer une pièce sur une case donnée
void Board::placePiece(Piece* piece, int x, int y)
{
    grid[x][y] = piece;
}

// Vérifier si le jeu est terminé
std::pair<bool, int> Board::is_game_over()
{
    int  whoWin         = 0; // 0 = personne, 1 = noir, 2 = blanc
    bool whiteKingAlive = false;
    bool blackKingAlive = false;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = grid[x][y];
            if (piece != nullptr)
            {
                if (piece->get_type() == "White King" && piece->get_state() == Piece::State::Alive)
                {
                    whiteKingAlive = true;
                }
                else if (piece->get_type() == "Black King" && piece->get_state() == Piece::State::Alive)
                {
                    blackKingAlive = true;
                }
            }
        }
    }

    // Si l'un des rois est mort, la partie est finie
    if (!whiteKingAlive)
    {
        whoWin = 1;            // les noirs ont gagné
        return {true, whoWin}; // La partie est terminée
    }
    if (!blackKingAlive)
    {
        whoWin = 2;            // les blancs ont gagné
        return {true, whoWin}; // La partie est terminée
    }

    return {false, whoWin}; // La partie n'est pas terminée
}

// Afficher le plateau
void Board::render()
{
    handleRandom();
    renderBoardSquares();
    renderPieces();

    if (selected_piece)
    {
        renderPossibleMoves();
        renderSelectionInfo();
    }

    handlePromotionPopup();
    handleGameOverPopup();
}

// Dessiner les cases de l'échiquier
void Board::renderBoardSquares()
{
    // double randomColor = (globalRandom.uniformContinuous(10, 255));
    for (int y = 7; y >= 0; --y)
    {
        for (int x = 0; x < 8; ++x)
        {
            ImVec2 pos = board_pos + ImVec2(x * square_size, (7 - y) * square_size);

            // Dessiner les cases
            ImU32 color = (x + y) % 2 == 0 ? squareColor : IM_COL32(240, 217, 181, 255);
            ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + square_size, pos.y + square_size), color);
        }
    }
}

// Afficher les pièces sur l'échiquier
void Board::renderPieces()
{
    for (int y = 7; y >= 0; --y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece* piece = grid[x][y];
            if (piece != nullptr)
            {
                renderPieceAt(piece, x, y);
            }
        }
    }
}

// Afficher une pièce à une position spécifique
void Board::renderPieceAt(Piece* piece, int x, int y)
{
    ImVec2      pos       = board_pos + ImVec2(x * square_size, (7 - y) * square_size);
    std::string piece_str = piece->get_name();

    // Dessiner la pièce
    ImGui::SetCursorScreenPos(pos);
    std::string button_label = piece_str + "##" + std::to_string(x) + std::to_string(y);

    // ImGui::PushFont(font3);
    ImGui::PushFont(chosenFont);

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Text, piece->get_color() == Piece::Color::White ? IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255));

    if (ImGui::Button(button_label.c_str(), ImVec2(square_size, square_size)))
    {
        handlePieceSelection(x, y);
    }

    ImGui::PopStyleColor(2);
    ImGui::PopFont();
}

// Gérer la sélection d'une pièce
void Board::handlePieceSelection(int x, int y)
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

// Afficher les mouvements possibles pour la pièce sélectionnée
void Board::renderPossibleMoves()
{
    Piece* piece = grid[selected_pos.first][selected_pos.second];
    if (piece != nullptr)
    {
        // Obtenir les mouvements possibles de la pièce
        std::vector<std::pair<int, int>> moves = piece->possible_moves(grid);

        // std::cout << "Mouvements possibles pour la pièce sélectionnée: ("
        //           << selected_pos.first << ", " << selected_pos.second << ")\n";

        for (const auto& move : moves)
        {
            renderMoveIndicator(piece, move);
            handleMoveSelection(move);
        }
    }
}

// Afficher l'indicateur visuel pour un mouvement possible
void Board::renderMoveIndicator(Piece* piece, const std::pair<int, int>& move)
{
    // Calculer la position du cercle de mouvement (en tenant compte de l'inversion de y)
    ImVec2 move_pos = board_pos + ImVec2(move.first * square_size, (7 - move.second) * square_size);

    // Vérifier si une pièce est déjà présente sur la case
    if (!is_piece_at(move)) // Si la case n'est pas occupée par une pièce
    {
        if (piece->get_color() == last_moved_piece_color)
        {
            // Dessiner un cercle vert transparent à la position du mouvement possible mais pas jouable
            ImGui::GetWindowDrawList()->AddCircleFilled(
                ImVec2(move_pos.x + square_size / 2, move_pos.y + square_size / 2),
                10.0f, IM_COL32(0, 100, 0, 100)
            );
        }
        else
        {
            // Dessiner un cercle vert à la position du mouvement possible
            ImGui::GetWindowDrawList()->AddCircleFilled(
                ImVec2(move_pos.x + square_size / 2, move_pos.y + square_size / 2),
                10.0f, IM_COL32(0, 255, 0, 255)
            );
        }
    }
}

// Gérer la sélection d'un mouvement par clic
void Board::handleMoveSelection(const std::pair<int, int>& move)
{
    // Calculer la position du mouvement sur l'écran
    ImVec2 move_pos = board_pos + ImVec2(move.first * square_size, (7 - move.second) * square_size);

    // Gérer le clic pour déplacer la pièce
    if (ImGui::IsMouseHoveringRect(move_pos, ImVec2(move_pos.x + square_size, move_pos.y + square_size))
        && ImGui::IsMouseClicked(0))
    {
        std::cout << "Déplacement effectué vers (" << move.first << ", " << move.second << ")\n";
        movePiece(selected_pos, move);
        selected_piece = false; // Désélectionner après déplacement
    }
}

// Afficher les informations sur la sélection actuelle
void Board::renderSelectionInfo()
{
    std::string msg = "Case sélectionnée : ("
                      + std::to_string(selected_pos.first)
                      + ", " + std::to_string(selected_pos.second) + ")";
    ImGui::Text("%s", msg.c_str());
}

// Gérer la popup de promotion d'un pion
void Board::handlePromotionPopup()
{
    if (promotion_active)
    {
        ImGui::OpenPopup("Promotion");
    }

    if (ImGui::BeginPopupModal("Promotion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Choisissez une promotion:");

        if (ImGui::Button("Queen"))
        {
            promotePawn("Queen", false);
        }
        if (ImGui::Button("Rook"))
        {
            promotePawn("Rook", false);
        }
        if (ImGui::Button("Bishop"))
        {
            promotePawn("Bishop", false);
        }
        if (ImGui::Button("Knight"))
        {
            promotePawn("Knight", false);
        }
        if (ImGui::Button("Aléatoir ;)"))
        {
            promotePawn("Knight", true);
        }

        ImGui::EndPopup();
    }
}

// Promouvoir un pion avec le type spécifié
void Board::promotePawn(const std::string& pieceType, bool aleat)
{
    promoted_piece->promote(pieceType, aleat);
    promotion_active = false;
    ImGui::CloseCurrentPopup();
}

// Gérer la popup de fin de partie
// void Board::handleGameOverPopup()
// {
//     if (is_game_over())
//     {
//         ImGui::OpenPopup("Game Over");
//     }

//     if (ImGui::BeginPopupModal("Game Over", NULL, ImGuiWindowFlags_AlwaysAutoResize))
//     {
//         ImGui::Text("Le roi a été capturé ! La partie est terminée.");

//         if (ImGui::Button("Quitter"))
//         {
//             exit(0); // Ferme le programme
//         }

//         ImGui::EndPopup();
//     }
// }

void Board::handleGameOverPopup()
{
    if (is_game_over().first)
    {
        std::string winner;
        switch (is_game_over().second)
        {
        case 1: winner = "Black"; break;
        case 2: winner = "White"; break;
        default: break;
        };

        // Obtenir les dimensions de la fenêtre
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 windowPos  = ImGui::GetWindowPos();

        // Créer un fond semi-transparent sur tout l'écran
        ImGui::GetWindowDrawList()->AddRectFilled(
            windowPos,
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            IM_COL32(0, 0, 0, 200) // Noir semi-transparent
        );

        // Calculer la position pour le texte de fin de jeu
        ImVec2 textPos = ImVec2(
            windowPos.x + windowSize.x * 0.5f,
            windowPos.y + windowSize.y * 0.4f
        );

        // Taille du texte et couleur
        float textSize    = 72.0f;
        ImU32 textColor   = winner == "White" ? IM_COL32(220, 220, 220, 255) : IM_COL32(50, 50, 50, 255);
        ImU32 shadowColor = IM_COL32(0, 0, 0, 150);

        // Texte principal avec effet d'ombre
        std::string gameOverText = winner + " Wins!";

        // // Ajouter l'ombre du texte
        // ImGui::GetWindowDrawList()->AddText(
        //     chosenFont,
        //     textSize,
        //     ImVec2(textPos.x - textSize * 2.5f + 3, textPos.y + 3), // Décalage de l'ombre
        //     shadowColor,
        //     gameOverText.c_str()
        // );

        // Ajouter le texte principal
        ImGui::GetWindowDrawList()->AddText(
            chosenFont,
            textSize,
            ImVec2(textPos.x - textSize * 2.5f, textPos.y),
            textColor,
            gameOverText.c_str()
        );

        // Ajouter un bouton pour quitter
        ImVec2 buttonPos = ImVec2(
            windowPos.x + windowSize.x * 0.5f - 50,
            windowPos.y + windowSize.y * 0.6f
        );

        ImGui::SetCursorScreenPos(buttonPos);
        if (ImGui::Button("Quit Game", ImVec2(100, 40)))
        {
            exit(0); // Ferme le programme
        }
    }
}

// Fonction pour déplacer une pièce d'une case à une autre
bool Board::movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to)
{
    Piece* piece = grid[from.first][from.second];
    if (piece == nullptr)
        return false;

    if (!isValidMove(piece, from, to))
        return false;

    // Capturer la pièce sur la case cible si elle existe
    capturePieceIfPresent(to);

    // Effectuer le déplacement
    performMove(piece, from, to);

    // Vérifier si un pion peut être promu
    if (checkForPawnPromotion(piece, to))
        return true;

    return true; // Mouvement valide, on a effectué le déplacement
}

// Vérifier si un mouvement est valide
bool Board::isValidMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to)
{
    std::vector<std::pair<int, int>> moves = piece->possible_moves(grid);
    if (std::find(moves.begin(), moves.end(), to) == moves.end() || piece->get_color() == last_moved_piece_color)
    {
        // std::cout << "Mouvement invalide pour " << piece->get_type()
        //           << " de (" << from.first << "," << from.second << ")"
        //           << " à (" << to.first << "," << to.second << ")" << std::endl;
        return false; // Mouvement invalide
    }
    return true;
}

// Capturer une pièce si elle est présente à la position spécifiée
void Board::capturePieceIfPresent(const std::pair<int, int>& position)
{
    Piece* target_piece = grid[position.first][position.second];
    if (target_piece != nullptr)
    {
        // La pièce cible existe et doit être capturée
        target_piece->set_state(Piece::State::Dead);
        // std::cout << "Capture de la pièce à (" << position.first << ", " << position.second << ")\n";
    }
}

// Effectuer le déplacement d'une pièce
void Board::performMove(Piece* piece, const std::pair<int, int>& from, const std::pair<int, int>& to)
{
    piece->move(to, grid);                   // Effectuer le mouvement dans la pièce
    grid[to.first][to.second]     = piece;   // Mettre à jour la grille
    grid[from.first][from.second] = nullptr; // Libérer la case d'origine

    // Met à jour la dernière couleur déplacée
    last_moved_piece_color = piece->get_color();
}

// Vérifier si un pion peut être promu
bool Board::checkForPawnPromotion(Piece* piece, const std::pair<int, int>& position)
{
    if (((piece->get_type() == "White Pawn" || piece->get_type() == "Black Pawn") && (position.second == 0 || position.second == 7)) && !is_game_over().first)
    {
        std::cout << "Le pion atteint la dernière ligne et peut être promu !" << std::endl;
        // Activer la promotion
        promotion_active = true;
        promotion_pos    = position;
        promoted_piece   = piece;
        return true; // Ne pas continuer tant que la promotion n'est pas choisie
    }
    return false;
}

// Informe si une pièce est présente sur une case donnée
bool Board::is_piece_at(const std::pair<int, int>& pos)
{
    return grid[pos.first][pos.second] != nullptr;
}

void Board::handleRandom()
{
    ImGui::SetCursorPos(ImVec2(30, 30)); // Positionnez le bouton où vous voulez
    if (ImGui::Button(activate_random ? "Disable Random Mode" : "Enable Random Mode"))
    {
        activate_random = !activate_random;
        std::cout << "Random mode: " << (activate_random ? "enabled" : "disabled") << std::endl;
    }
}
