#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QObject>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "cell.h"

class Minesweeper : public QObject {
    Q_OBJECT

public:
    Minesweeper(QObject *parent = nullptr);
    Minesweeper(int width, int height, int mines);
    void initializeGame(int width = 10, int height = 10, int mineCount = 10);
    bool revealCell(int x, int y);
    bool isGameOver() const;
    const std::vector<std::vector<Cell>>& getCells() const;

signals:
    void gameOver(bool won);

private:
    void placeMines();
    void calculateNumbers();

    int width;
    int height;
    int mines;
    bool _gameOver;
    std::vector<std::vector<Cell>> cells;
};

#endif // MINESWEEPER_H