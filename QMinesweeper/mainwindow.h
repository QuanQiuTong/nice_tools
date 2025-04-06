#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
#include "minesweeper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startGame();
    void onGameOver(bool won);
    void onCellClicked();
    void onCellRightClicked();
    void onCellDoubleClicked();

private:
    void createMineField();
    void clearMineField();
    void updateCell(int x, int y);
    void showTemporaryX(int x, int y);

    Ui::MainWindow *ui;
    Minesweeper *minesweeper;
    QGridLayout *gridLayout;
    std::vector<std::vector<QPushButton*>> buttons;
    int gridWidth;
    int gridHeight;
};

#endif // MAINWINDOW_H