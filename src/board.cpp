#include "board.h"

Board::Board(bool& vsAI) : vsAI(vsAI)
{
    setupInitialBoard();
    selectedSquare = -1;
    isPlayer1Turn = true;
    mustContinueJump = false;
    aiTimer = 0.0;
}

void Board::setupInitialBoard()
{
    boardState.fill(0);
    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int col = 0; col < BOARD_SIZE; col++)
        {
            if((row + col) % 2 != 0)
            {
                int index = row * BOARD_SIZE + col;
                if(row < 3)
                    boardState[index] = 2;
                else if(row > 4)
                    boardState[index] = 1;
            }
        }
    }
}

State Board::update()
{
    if(IsKeyPressed(KEY_ESCAPE))
        return State::MENU;

    if(!hasValidMoves())
    {
        winner = isPlayer1Turn ? 2 : 1;
        return State::GAMEOVER;
    }

    if(isPlayer1Turn || !vsAI)
    {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            Vector2 mousePos = GetMousePosition();
            int clickedIndex = getIndexFromMouse(mousePos);

            if(clickedIndex != -1)
            {
                int clickedPiece = boardState.at(clickedIndex);

                if(isCurrentPlayerPiece(clickedPiece) && !mustContinueJump)
                {
                    bool globalCapture = hasAnyCapture();
                    if(!globalCapture || !getCapturesForSquare(clickedIndex).empty())
                    {
                        selectedSquare = clickedIndex;
                        updateValidTargets();
                    }
                }
                else if(selectedSquare != -1)
                {
                    for(int target : validTargets)
                    {
                        if(clickedIndex == target)
                        {
                            int from = selectedSquare;
                            int to = clickedIndex;
                            int piece = boardState.at(from);
                            bool isCapture = (std::abs(from / 8 - to / 8) == 2);

                            boardState[from] = 0;
                            if(isCapture)
                            {
                                int middle = (from + to) / 2;
                                boardState[middle] = 0;
                            }

                            if(piece == 1 && (to / 8) == 0)
                                piece = 3;
                            else if(piece == 2 && (to / 8) == 7)
                                piece = 4;

                            boardState[to] = piece;

                            if(isCapture && !getCapturesForSquare(to).empty())
                            {
                                mustContinueJump = true;
                                selectedSquare = to;
                                updateValidTargets();
                            }
                            else
                            {
                                mustContinueJump = false;
                                selectedSquare = -1;
                                validTargets.clear();
                                isPlayer1Turn = !isPlayer1Turn;
                                aiTimer = GetTime();
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    else if (vsAI && !isPlayer1Turn)
    {
        if(GetTime() - aiTimer > 0.5)
        {
            std::vector<int> aiPath = ai.getBestMove(boardState, CheckersAI::Strategy::Minimax, 5);

            if(!aiPath.empty())
            {
                int startPos = aiPath.front();
                int piece = boardState.at(startPos);
                boardState[startPos] = 0;

                for(size_t i = 0; i < aiPath.size() - 1; i++)
                {
                    int from = aiPath.at(i);
                    int to = aiPath.at(i + 1);
                    int rowFrom = from / 8;
                    int rowTo = to / 8;

                    if(std::abs(rowFrom - rowTo) == 2)
                    {
                        int capturedIndex = (from + to) / 2;
                        boardState[capturedIndex] = 0;
                    }
                }

                int finalPos = aiPath.back();
                if(piece == 2 && (finalPos / 8) == 7)
                    piece = 4;
                boardState[finalPos] = piece;
                isPlayer1Turn = true;
            }
            else
            {
                winner = 1;
                return State::GAMEOVER;
            }
        }
    }

    return State::GAME;
}

void Board::draw()
{
    ClearBackground(RAYWHITE);

    float tileSize = getTileSize();
    float offsetX = getOffsetX();
    float offsetY = getOffsetY();

    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int col = 0; col < BOARD_SIZE; col++)
        {
            Color tileColor = ((row + col) % 2 == 0) ? colorLight : colorDark;
            DrawRectangle(offsetX + col * tileSize, offsetY + row * tileSize, tileSize, tileSize, tileColor);
        }
    }

    if(selectedSquare != -1)
    {
        int col = selectedSquare % BOARD_SIZE;
        int row = selectedSquare / BOARD_SIZE;
        DrawRectangle(offsetX + col * tileSize, offsetY + row * tileSize, tileSize, tileSize, colorHighlight);
    }

    for(int target : validTargets)
    {
        int col = target % BOARD_SIZE;
        int row = target / BOARD_SIZE;
        DrawRectangle(offsetX + col * tileSize, offsetY + row * tileSize, tileSize, tileSize, colorValidTarget);
        DrawCircle(offsetX + col * tileSize + tileSize / 2.0f, offsetY + row * tileSize + tileSize / 2.0f, tileSize * 0.12f, GREEN);
    }

    for(int i = 0; i < 64; i++)
    {
        if(boardState.at(i) != 0)
        {
            Vector2 center = getCenterFromIndex(i);
            float radius = tileSize * 0.4f;

            Color pieceColor = isPlayer1Piece(boardState[i]) ? colorPlayer1 : colorPlayer2;
            DrawCircleV(center, radius, pieceColor);

            if(boardState.at(i) == 3 || boardState.at(i) == 4)
                DrawCircleV(center, radius * 0.4f, GOLD);
        }
    }
}

bool Board::isPlayer1Piece(int piece) const
{
    return piece == 1 || piece == 3;
}

bool Board::isPlayer2Piece(int piece) const
{
    return piece == 2 || piece == 4;
}

bool Board::isCurrentPlayerPiece(int piece) const
{
    return isPlayer1Turn ? isPlayer1Piece(piece) : isPlayer2Piece(piece);
}

bool Board::isOpponentPiece(int piece) const
{
    return isPlayer1Turn ? isPlayer2Piece(piece) : isPlayer1Piece(piece);
}

std::vector<std::pair<int, int> > Board::getPieceDirections(int piece) const
{
    if(piece == 3 || piece == 4)
        return {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    if(piece == 1)
        return {{-1, -1}, {-1, 1}};
    if(piece == 2)
        return {{1, -1}, {1, 1}};
    return {};
}

int Board::getIndexFromMouse(Vector2 mousePos) const
{
    float tileSize = getTileSize();
    float offsetX = getOffsetX();
    float offsetY = getOffsetY();

    int col = (mousePos.x - offsetX) / tileSize;
    int row = (mousePos.y - offsetY) / tileSize;

    if(col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
        return row * BOARD_SIZE + col;

    return -1;
}

Vector2 Board::getCenterFromIndex(int index) const
{
    float tileSize = getTileSize();
    int col = index % BOARD_SIZE;
    int row = index / BOARD_SIZE;

    return {
        getOffsetX() + col * tileSize + tileSize / 2.0f,
        getOffsetY() + row * tileSize + tileSize / 2.0f
    };
}

std::vector<int> Board::getCapturesForSquare(int index) const
{
    std::vector<int> targets;
    int piece = boardState.at(index);
    if(!isCurrentPlayerPiece(piece))
        return targets;

    int row = index / 8;
    int col = index % 8;

    for(auto [dRow, dCol] : getPieceDirections(piece))
    {
        int midRow = row + dRow;
        int midCol = col + dCol;
        int targetRow = row + 2 * dRow;
        int targetCol = col + 2 * dCol;

        if(targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
        {
            int middle = midRow * 8 + midCol;
            int target = targetRow * 8 + targetCol;

            if(isOpponentPiece(boardState.at(middle)) && boardState.at(target) == 0)
                targets.push_back(target);
        }
    }
    return targets;
}

std::vector<int> Board::getNormalMovesForSquare(int index) const
{
    std::vector<int> targets;
    int piece = boardState.at(index);
    if(!isCurrentPlayerPiece(piece))
        return targets;

    int row = index / 8;
    int col = index % 8;

    for(auto [dRow, dCol] : getPieceDirections(piece))
    {
        int targetRow = row + dRow;
        int targetCol = col + dCol;

        if(targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
        {
            int target = targetRow * 8 + targetCol;
            if(boardState.at(target) == 0)
                targets.push_back(target);
        }
    }
    return targets;
}

bool Board::hasValidMoves() const
{
    if(mustContinueJump)
        return true;

    for(int i = 0; i < 64; i++)
    {
        if(isCurrentPlayerPiece(boardState.at(i)))
        {
            if(!getCapturesForSquare(i).empty() || !getNormalMovesForSquare(i).empty())
                return true;
        }
    }
    return false;
}

bool Board::hasAnyCapture() const
{
    for(int i = 0; i < 64; i++)
    {
        if(isCurrentPlayerPiece(boardState.at(i)))
            if(!getCapturesForSquare(i).empty())
                return true;
    }
    return false;
}

void Board::updateValidTargets()
{
    validTargets.clear();
    if(selectedSquare == -1)
        return;

    bool globalCaptureAvailable = hasAnyCapture();
    auto captures = getCapturesForSquare(selectedSquare);

    if(globalCaptureAvailable)
        validTargets = captures;
    else if(!mustContinueJump)
            validTargets = getNormalMovesForSquare(selectedSquare);
}
float Board::getTileSize() const
{
    return std::min(GetScreenWidth(), GetScreenHeight()) / (float)BOARD_SIZE;
}
float Board::getOffsetX() const
{
    return (GetScreenWidth() - (getTileSize() * BOARD_SIZE)) / 2.0f;
}
float Board::getOffsetY() const
{
    return (GetScreenHeight() - (getTileSize() * BOARD_SIZE)) / 2.0f;
}
