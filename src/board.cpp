#include "board.h"

Board::Board()
{
    setupInitialBoard();
    selectedSquare = -1;
    isPlayerTurn = true;
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

    if(isPlayerTurn)
    {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            Vector2 mousePos = GetMousePosition();
            int clickedIndex = getIndexFromMouse(mousePos);

            if(clickedIndex != -1)
            {
                int clickedPiece = boardState.at(clickedIndex);
                if(isHumanPiece(clickedPiece) && !mustContinueJump)
                {
                    bool globalCapture = hasAnyHumanCapture();
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
                                isPlayerTurn = false;
                                aiTimer = GetTime();
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(GetTime() - aiTimer > 0.5)
        {
            std::vector<int> aiPath = ai.getBestMove(boardState);

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
                isPlayerTurn = true;
            }
            else
            {
                winner = aiPath.empty() ? 1 : 2;
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

            Color pieceColor = isHumanPiece(boardState[i]) ? colorHuman : colorAI;
            DrawCircleV(center, radius, pieceColor);

            if(boardState.at(i) == 3 || boardState.at(i) == 4)
                DrawCircleV(center, radius * 0.4f, GOLD);
        }
    }
}

bool Board::isHumanPiece(int piece) const
{
    return piece == 1 || piece == 3;
}

bool Board::isAIPiece(int piece) const
{
    return piece == 2 || piece == 4;
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

std::vector<std::pair<int, int>> Board::getHumanDirections(int piece) const
{
    if(piece == 3)
        return {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    return {{-1, -1}, {-1, 1}};
}

std::vector<int> Board::getCapturesForSquare(int index) const
{
    std::vector<int> targets;
    int piece = boardState.at(index);
    if(!isHumanPiece(piece))
        return targets;

    int row = index / 8;
    int col = index % 8;

    for(auto [dRow, dCol] : getHumanDirections(piece))
    {
        int midRow = row + dRow;
        int midCol = col + dCol;
        int targetRow = row + 2 * dRow;
        int targetCol = col + 2 * dCol;

        if(targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
        {
            int middle = midRow * 8 + midCol;
            int target = targetRow * 8 + targetCol;

            if(isAIPiece(boardState.at(middle)) && boardState.at(target) == 0)
                targets.push_back(target);
        }
    }
    return targets;
}

std::vector<int> Board::getNormalMovesForSquare(int index) const
{
    std::vector<int> targets;
    int piece = boardState.at(index);
    if(!isHumanPiece(piece))
        return targets;

    int row = index / 8;
    int col = index % 8;

    for(auto [dRow, dCol] : getHumanDirections(piece))
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

bool Board::hasAnyHumanCapture() const
{
    for(int i = 0; i < 64; i++)
    {
        if(isHumanPiece(boardState.at(i)))
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

    bool globalCaptureAvailable = hasAnyHumanCapture();
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
