#include "minesweeper.h"
#include <vector>
#include <cstdlib>
#include <ctime>

Minesweeper::Minesweeper(QObject *parent)
{
    Q_UNUSED(parent);
    width = 10;
    height = 10;
    mines = 10;
    _gameOver = false;
    cells.resize(width, std::vector<Cell>(height));
    std::srand(std::time(nullptr));
    placeMines();
    calculateNumbers();
}

Minesweeper::Minesweeper(int width, int height, int mines)
    : width(width), height(height), mines(mines), _gameOver(false), cells(width, std::vector<Cell>(height))
{
    std::srand(std::time(nullptr));
    placeMines();
    calculateNumbers();
}

void Minesweeper::initializeGame(int width, int height, int mineCount)
{
    this->width = width;
    this->height = height;
    this->mines = mineCount;
    _gameOver = false;
    cells.assign(width, std::vector<Cell>(height));
    placeMines();
    calculateNumbers();
}

void Minesweeper::placeMines()
{
    int placedMines = 0;
    while (placedMines < mines) {
        int x = std::rand() % width;
        int y = std::rand() % height;
        if (!cells[x][y].hasMine()) {
            cells[x][y].setMine(true);
            placedMines++;
        }
    }
}

void Minesweeper::calculateNumbers()
{
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (cells[x][y].hasMine()) {
                continue;
            }
            int mineCount = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && cells[nx][ny].hasMine()) {
                        mineCount++;
                    }
                }
            }
            cells[x][y].setNumber(mineCount);
        }
    }
}

bool Minesweeper::revealCell(int x, int y)
{
    // 边界检查
    if (_gameOver || x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    // 如果已经揭示过或已标记，则跳过
    if (cells[x][y].isRevealed() || cells[x][y].isFlagged()) {
        return false;
    }
    
    // 如果是地雷，游戏结束
    if (cells[x][y].hasMine()) {
        _gameOver = true;
        emit gameOver(false); // 触发游戏结束信号，玩家失败
        return false;
    }
    
    // 揭示当前单元格
    cells[x][y].setRevealed(true);
    
    // 如果是空白格子（周围没有地雷），则揭示周围的格子
    if (cells[x][y].getNumber() == 0) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue; // 跳过当前格子
                revealCell(x + dx, y + dy);
            }
        }
    }

    // 检查是否获胜
    bool allNonMinesRevealed = true;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (!cells[i][j].hasMine() && !cells[i][j].isRevealed()) {
                allNonMinesRevealed = false;
                break;
            }
        }
        if (!allNonMinesRevealed) break;
    }
    
    if (allNonMinesRevealed) {
        _gameOver = true;
        emit gameOver(true); // 玩家获胜
    }

    return true;
}

bool Minesweeper::isGameOver() const
{
    return _gameOver;
}

const std::vector<std::vector<Cell>>& Minesweeper::getCells() const
{
    return cells;
}