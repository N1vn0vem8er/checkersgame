#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <raylib.h>
#include "checkersai.h"
#include "state.h"

class Board
{
public:
    Board(bool& vsAI);
    State update();
    void draw();
    int winner {0};

private:
    std::array<int, 64> boardState;
    CheckersAI ai;
    double aiTimer;
    int selectedSquare;
    bool isPlayer1Turn;
    bool mustContinueJump;
    std::vector<int> validTargets;
    static constexpr int TILE_SIZE = 100;
    static constexpr int BOARD_SIZE = 8;
    const Color colorLight {240, 217, 181, 255};
    const Color colorDark {181, 136, 99, 255};
    const Color colorPlayer1 {RED};
    const Color colorPlayer2 {BLACK};
    const Color colorHighlight {255, 255, 0, 100};
    const Color colorValidTarget {0, 255, 0, 120};
    bool& vsAI;

private:
    bool isPlayer1Piece(int piece) const;
    bool isPlayer2Piece(int piece) const;
    bool isCurrentPlayerPiece(int piece) const;
    bool isOpponentPiece(int piece) const;
    std::vector<std::pair<int, int>> getPieceDirections(int piece) const;
    std::vector<int> getCapturesForSquare(int index) const;
    std::vector<int> getNormalMovesForSquare(int index) const;
    bool hasAnyCapture() const;
    bool hasValidMoves() const;
    void updateValidTargets();
    void setupInitialBoard();
    int getIndexFromMouse(Vector2 mousePos) const;
    Vector2 getCenterFromIndex(int index) const;
    float getTileSize() const;
    float getOffsetX() const;
    float getOffsetY() const;
};

#endif // BOARD_H
