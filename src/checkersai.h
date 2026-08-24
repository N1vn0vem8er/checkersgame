#ifndef CHECKERSAI_H
#define CHECKERSAI_H

#include <array>
#include <vector>
#include <random>

class CheckersAI
{
public:
    enum class Strategy {
        Random,
        Minimax
    };
    using Board = std::array<int, 64>;
    using Move = std::vector<int>;
    CheckersAI();
    Move getBestMove(const Board& board);
    Move getBestMove(const Board& board, Strategy strategy, int maxDepth = 4);
    Move getBestMoveMinimax(const Board& board, int maxDepth = 4);

private:
    static constexpr int EMPTY = 0;
    static constexpr int HUMAN_MAN = 1;
    static constexpr int AI_MAN = 2;
    static constexpr int HUMAN_KING = 3;
    static constexpr int AI_KING = 4;
    std::mt19937 rng;

private:
    bool isAIPiece(int piece) const;
    bool isHumanPiece(int piece) const;
    std::vector<Move> getNormalMoves(const Board& board);
    std::vector<std::pair<int, int>> getDirections(int piece) const;
    Move getRandomMove(const std::vector<Move>& moves);
    void findJumps(Board currentBoard, int currentPos, Move currentPath, std::vector<Move>& allPaths);
    int evaluateBoard(const Board& board) const;
    Board applyMove(const Board& board, const Move& move) const;
    std::vector<Move> getAllLegalMoves(const Board& board, bool isAI) const;
    void findJumpsGeneric(Board currentBoard, int currentPos, Move currentPath, std::vector<Move>& allPaths, bool isAI) const;
    std::vector<std::pair<int, int>> getDirectionsGeneric(int piece) const;
    int minimax(Board board, int depth, int alpha, int beta, bool isMaximizing);
};

#endif // CHECKERSAI_H
