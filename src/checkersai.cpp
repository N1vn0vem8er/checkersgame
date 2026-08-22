#include "checkersai.h"

CheckersAI::CheckersAI()
{
    std::random_device rd;
    rng.seed(rd());
}

bool CheckersAI::isAIPiece(int piece) const
{
    return piece == AI_MAN || piece == AI_KING;
}

bool CheckersAI::isHumanPiece(int piece) const
{
    return piece == HUMAN_MAN || piece == HUMAN_KING;
}

void CheckersAI::findJumps(Board currentBoard, int currentPos, Move currentPath, std::vector<Move> &allPaths)
{
    bool jumpFound = false;
    int piece = currentBoard.at(currentPos);
    int row = currentPos / 8;
    int col = currentPos % 8;
    auto directions = getDirections(piece);

    for(auto [dRow, dCol] : directions)
    {
        int midRow = row + dRow;
        int midCol = col + dCol;
        int targetRow = row + 2 * dRow;
        int targetCol = col + 2 * dCol;

        if(targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
        {
            int middle = midRow * 8 + midCol;
            int target = targetRow * 8 + targetCol;

            if(isHumanPiece(currentBoard.at(middle)) && currentBoard.at(target) == EMPTY)
            {
                jumpFound = true;
                Board nextBoard = currentBoard;
                nextBoard[currentPos] = EMPTY;
                nextBoard[middle] = EMPTY;

                int newPiece = piece;
                if(piece == AI_MAN && targetRow == 7)
                    newPiece = AI_KING;
                nextBoard[target] = newPiece;

                Move nextPath = currentPath;
                nextPath.push_back(target);
                findJumps(nextBoard, target, nextPath, allPaths);
            }
        }
    }

    if(!jumpFound && currentPath.size() > 1)
        allPaths.push_back(currentPath);
}

CheckersAI::Move CheckersAI::getRandomMove(const std::vector<Move> &moves)
{
    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    return moves[dist(rng)];
}

std::vector<CheckersAI::Move> CheckersAI::getNormalMoves(const Board &board)
{
    std::vector<Move> moves;
    for(int i = 0; i < 64; i++)
    {
        if(board.at(i) == 2)
        {
            int row = i / 8;
            int col = i % 8;
            if(row < 7 && col > 0)
            {
                int target = (row + 1) * 8 + (col - 1);
                if(board.at(target) == 0)
                    moves.push_back({i, target});
            }
            if(row < 7 && col < 7)
            {
                int target = (row + 1) * 8 + (col + 1);
                if(board.at(target) == 0)
                    moves.push_back({i, target});
            }
        }
    }
    return moves;
}

std::vector<std::pair<int, int>> CheckersAI::getDirections(int piece) const
{
    if(piece == AI_KING)
        return {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    else
        return {{1, -1}, {1, 1}};
}

CheckersAI::Move CheckersAI::getBestMove(const Board &board)
{
    std::vector<Move> captures;
    for(int i = 0; i < 64; i++)
    {
        if(isAIPiece(board.at(i)))
        {
            Move currentPath = {i};
            findJumps(board, i, currentPath, captures);
        }
    }

    if(!captures.empty())
        return getRandomMove(captures);

    std::vector<Move> normalMoves = getNormalMoves(board);
    if(!normalMoves.empty())
        return getRandomMove(normalMoves);

    return {};
}
