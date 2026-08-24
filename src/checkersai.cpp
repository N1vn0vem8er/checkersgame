#include "checkersai.h"

CheckersAI::CheckersAI()
{
    std::random_device rd;
    rng.seed(rd());
}

CheckersAI::Move CheckersAI::getBestMoveMinimax(const Board &board, int maxDepth)
{
    std::vector<Move> moves = getAllLegalMoves(board, true);
    if(moves.empty())
        return {};

    Move bestMove = moves[0];
    int bestValue = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    for(const auto &move : moves)
    {
        Board newBoard = applyMove(board, move);
        int moveValue = minimax(newBoard, maxDepth - 1, alpha, beta, false);

        if(moveValue > bestValue)
        {
            bestValue = moveValue;
            bestMove = move;
        }
        alpha = std::max(alpha, bestValue);
    }

    return bestMove;
}

CheckersAI::Move CheckersAI::getBestMove(const Board &board, Strategy strategy, int maxDepth)
{
    if(strategy == Strategy::Minimax)
    {
        return getBestMoveMinimax(board, maxDepth);
    }
    return getBestMove(board);
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

int CheckersAI::evaluateBoard(const Board &board) const
{
    int score = 0;
    for(int i = 0; i < 64; i++)
    {
        switch(board.at(i))
        {
        case AI_MAN:
            score += 10;
            break;
        case AI_KING:
            score += 30;
            break;
        case HUMAN_MAN:
            score -= 10;
            break;
        case HUMAN_KING:
            score -= 30;
            break;
        }
    }
    return score;
}

void CheckersAI::findJumpsGeneric(Board currentBoard, int currentPos, Move currentPath, std::vector<Move> &allPaths, bool isAI) const
{
    bool jumpFound = false;
    int piece = currentBoard.at(currentPos);
    int row = currentPos / 8;
    int col = currentPos % 8;
    auto directions = getDirectionsGeneric(piece);

    for(auto [dRow, dCol] : directions)
    {
        int targetRow = row + 2 * dRow;
        int targetCol = col + 2 * dCol;

        if(targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
        {
            int middle = row + dRow * 8 + col + dCol;
            int target = targetRow * 8 + targetCol;

            bool isEnemy = isAI ? isHumanPiece(currentBoard.at(middle)) : isAIPiece(currentBoard.at(middle));

            if(isEnemy && currentBoard.at(target) == EMPTY)
            {
                jumpFound = true;
                Board nextBoard = currentBoard;
                nextBoard[currentPos] = EMPTY;
                nextBoard[middle] = EMPTY;

                int newPiece = piece;
                if(piece == AI_MAN && targetRow == 7)
                    newPiece = AI_KING;
                else if(piece == HUMAN_MAN && targetRow == 0)
                    newPiece = HUMAN_KING;

                nextBoard[target] = newPiece;

                Move nextPath = currentPath;
                nextPath.push_back(target);
                findJumpsGeneric(nextBoard, target, nextPath, allPaths, isAI);
            }
        }
    }

    if(!jumpFound && currentPath.size() > 1)
        allPaths.push_back(currentPath);
}

std::vector<std::pair<int, int> > CheckersAI::getDirectionsGeneric(int piece) const
{
    if(piece == AI_KING || piece == HUMAN_KING)
        return {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    else if(piece == AI_MAN)
        return {{1, -1}, {1, 1}};
    else if(piece == HUMAN_MAN)
        return {{-1, -1}, {-1, 1}};
    return {};
}

int CheckersAI::minimax(Board board, int depth, int alpha, int beta, bool isMaximizing)
{
    if(depth == 0)
    {
        return evaluateBoard(board);
    }

    std::vector<Move> legalMoves = getAllLegalMoves(board, isMaximizing);

    if(legalMoves.empty())
        return isMaximizing ? -10000 + (4 - depth) : 10000 - (4 - depth);

    if(isMaximizing)
    {
        int maxEval = std::numeric_limits<int>::min();
        for(const auto &move : legalMoves)
        {
            Board newBoard = applyMove(board, move);
            int eval = minimax(newBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if(beta <= alpha)
                break;
        }
        return maxEval;
    }
    else
    {
        int minEval = std::numeric_limits<int>::max();
        for(const auto &move : legalMoves)
        {
            Board newBoard = applyMove(board, move);
            int eval = minimax(newBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if(beta <= alpha)
                break;
        }
        return minEval;
    }
}

std::vector<CheckersAI::Move> CheckersAI::getAllLegalMoves(const Board &board, bool isAI) const
{
    std::vector<Move> captures;
    for(int i = 0; i < 64; i++)
    {
        int piece = board.at(i);
        if((isAI && isAIPiece(piece)) || (!isAI && isHumanPiece(piece)))
            findJumpsGeneric(board, i, {i}, captures, isAI);
    }

    if(!captures.empty())
        return captures;

    std::vector<Move> normalMoves;
    for(int i = 0; i < 64; i++)
    {
        int piece = board.at(i);
        if((isAI && isAIPiece(piece)) || (!isAI && isHumanPiece(piece)))
        {
            int row = i / 8;
            int col = i % 8;
            auto directions = getDirectionsGeneric(piece);

            for(auto [dRow, dCol] : directions)
            {
                int targetRow = row + dRow;
                int targetCol = col + dCol;

                if(targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
                {
                    int target = targetRow * 8 + targetCol;
                    if(board.at(target) == EMPTY)
                        normalMoves.push_back({i, target});
                }
            }
        }
    }

    return normalMoves;
}

CheckersAI::Board CheckersAI::applyMove(const Board &board, const Move &move) const
{
    Board newBoard = board;
    if(move.size() < 2)
        return newBoard;

    int piece = newBoard[move[0]];
    newBoard[move[0]] = EMPTY;

    for(size_t i = 1; i < move.size(); ++i)
    {
        int prev = move[i - 1];
        int curr = move[i];

        if(std::abs(curr / 8 - prev / 8) == 2)
        {
            int mid = (prev + curr) / 2;
            newBoard[mid] = EMPTY;
        }
        if(piece == AI_MAN && curr / 8 == 7)
            piece = AI_KING;
        else if(piece == HUMAN_MAN && curr / 8 == 0)
            piece = HUMAN_KING;
    }

    newBoard[move.back()] = piece;
    return newBoard;
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
