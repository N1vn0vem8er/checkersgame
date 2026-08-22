#ifndef CHECKERSAI_H
#define CHECKERSAI_H

#include <array>
#include <vector>
#include <random>

class CheckersAI
{
public:
    using Board = std::array<int, 64>;
    using Move = std::vector<int>;
    CheckersAI();
    Move getBestMove(const Board& board);

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
};

#endif // CHECKERSAI_H
