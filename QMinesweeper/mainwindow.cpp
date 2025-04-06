#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "minesweeper.h"
#include "cellbutton.h"
#include <QMessageBox>
#include <QTimer>
#include <QGridLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , minesweeper(new Minesweeper(this))
    , gridLayout(nullptr)
    , gridWidth(10)
    , gridHeight(10)
{
    ui->setupUi(this);
    
    // 创建一个widget来容纳网格布局
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建垂直布局来放置开始按钮和雷区
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(ui->startButton);
    
    // 创建网格布局
    gridLayout = new QGridLayout();
    mainLayout->addLayout(gridLayout);
    
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(minesweeper, &Minesweeper::gameOver, this, &MainWindow::onGameOver);
    
    // 设置窗口大小
    resize(400, 500);
}

MainWindow::~MainWindow()
{
    clearMineField();
    delete ui;
    delete minesweeper;
}

void MainWindow::startGame()
{
    std::cout << "Starting a new game..." << std::endl;
    
    // 清除旧的雷区
    clearMineField();
    
    // 初始化新游戏
    gridWidth = 10;
    gridHeight = 10;
    int mineCount = 10;
    minesweeper->initializeGame(gridWidth, gridHeight, mineCount);
    
    // 创建新的雷区UI
    createMineField();
}

void MainWindow::createMineField()
{
    // 调整按钮容器大小
    buttons.resize(gridWidth);
    for (int x = 0; x < gridWidth; ++x) {
        buttons[x].resize(gridHeight);
    }
    
    // 创建按钮网格
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            CellButton* button = new CellButton(x, y);
            button->setFixedSize(30, 30);
            button->setCheckable(false);
            
            // 连接按钮的信号
            connect(button, &QPushButton::clicked, this, &MainWindow::onCellClicked);
            connect(button, &CellButton::rightClicked, this, &MainWindow::onCellRightClicked);
            connect(button, &CellButton::doubleClicked, this, &MainWindow::onCellDoubleClicked);
            
            gridLayout->addWidget(button, y, x);
            buttons[x][y] = button;
        }
    }
}

void MainWindow::clearMineField()
{
    // 清除旧的按钮
    for (auto& row : buttons) {
        for (auto& button : row) {
            if (button) {
                gridLayout->removeWidget(button);
                delete button;
                button = nullptr;
            }
        }
    }
    buttons.clear();
}

void MainWindow::onCellClicked()
{
    CellButton* button = qobject_cast<CellButton*>(sender());
    if (button) {
        int x = button->getX();
        int y = button->getY();
        
        // 揭示单元格
        minesweeper->revealCell(x, y);
        
        // 更新UI - 不管是否成功揭示都更新
        const auto& cells = minesweeper->getCells();
        for (int i = 0; i < gridWidth; ++i) {
            for (int j = 0; j < gridHeight; ++j) {
                if (cells[i][j].isRevealed() || cells[i][j].hasMine() && minesweeper->isGameOver()) {
                    updateCell(i, j);
                }
            }
        }
    }
}

void MainWindow::onCellRightClicked()
{
    CellButton* button = qobject_cast<CellButton*>(sender());
    if (button) {
        int x = button->getX();
        int y = button->getY();
        
        // 获取当前单元格
        const auto& cells = minesweeper->getCells();
        if (!cells[x][y].isRevealed()) {
            // 切换标记状态
            bool flagged = !cells[x][y].isFlagged();
            const_cast<Cell&>(cells[x][y]).setFlagged(flagged);
            
            // 更新UI
            if (flagged) {
                button->setText("🚩");
            } else {
                button->setText("");
            }
        }
    }
}

void MainWindow::onCellDoubleClicked()
{
    std::cout << "Double clicked on cell" << std::endl;
    CellButton* button = qobject_cast<CellButton*>(sender());
    if (!button) return;
    
    int x = button->getX();
    int y = button->getY();
    
    const auto& cells = minesweeper->getCells();
    
    // 如果该格子未被揭开或不是数字，则不处理
    if (!cells[x][y].isRevealed() || cells[x][y].getNumber() == 0) {
        return;
    }
    
    // 获取格子周围的旗帜数量
    int flagCount = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight && 
                cells[nx][ny].isFlagged()) {
                flagCount++;
            }
        }
    }
    
    // 如果旗帜数量等于格子上的数字，则揭开周围未被揭开的格子
    if (flagCount == cells[x][y].getNumber()) {
        bool mineClicked = false;
        
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                
                int nx = x + dx;
                int ny = y + dy;
                
                // 只处理未被揭开且未标记的格子
                if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight && 
                    !cells[nx][ny].isRevealed() && !cells[nx][ny].isFlagged()) {
                    // 如果点到了地雷，记录下来
                    if (cells[nx][ny].hasMine()) {
                        mineClicked = true;
                    }
                    // 揭开该格子
                    minesweeper->revealCell(nx, ny);
                }
            }
        }
        
        // 更新UI
        const auto& updatedCells = minesweeper->getCells();
        for (int i = 0; i < gridWidth; ++i) {
            for (int j = 0; j < gridHeight; ++j) {
                if (updatedCells[i][j].isRevealed() || updatedCells[i][j].hasMine() && minesweeper->isGameOver()) {
                    updateCell(i, j);
                }
            }
        }
    } else {
        // 如果旗帜数量不等于格子上的数字，则显示一个临时的X
        showTemporaryX(x, y);
    }
}

// 添加临时显示X的方法
void MainWindow::showTemporaryX(int x, int y)
{
    QPushButton* button = buttons[x][y];
    QString originalText = button->text();
    QString originalStyleSheet = button->styleSheet();
    
    // 显示X
    button->setText("❌");
    
    // 1秒后恢复原样
    QTimer::singleShot(500, this, [this, button, originalText, originalStyleSheet]() {
        button->setText(originalText);
        button->setStyleSheet(originalStyleSheet);
    });
}

void MainWindow::updateCell(int x, int y)
{
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
        return;
    }
    
    QPushButton* button = buttons[x][y];
    const auto& cells = minesweeper->getCells();
    const Cell& cell = cells[x][y];
    
    if (cell.isRevealed()) {
        // 不要禁用按钮，这样能继续接收双击事件
        // button->setEnabled(false);
        
        if (cell.hasMine()) {
            button->setText("💣");
            button->setStyleSheet("background-color: red; border: 1px solid #aaa;");
        } else {
            int number = cell.getNumber();
            if (number > 0) {
                QString numberText = QString::number(number);
                button->setText(numberText);
                
                // 设置不同数字的颜色
                QString color;
                switch (number) {
                    case 1: color = "blue"; break;
                    case 2: color = "green"; break;
                    case 3: color = "red"; break;
                    case 4: color = "purple"; break;
                    case 5: color = "maroon"; break;
                    case 6: color = "turquoise"; break;
                    case 7: color = "black"; break;
                    case 8: color = "gray"; break;
                }
                // 修改样式，使边框颜色变浅
                button->setStyleSheet(QString("color: %1; background-color: #e0e0e0; border: 1px solid #ccc;").arg(color));
            } else {
                button->setText("");
                // 修改样式，使边框颜色变浅
                button->setStyleSheet("background-color: #e0e0e0; border: 1px solid #ccc;");
            }
        }
    }
}

void MainWindow::onGameOver(bool won)
{
    // 显示所有地雷
    const auto& cells = minesweeper->getCells();
    for (int x = 0; x < gridWidth; ++x) {
        for (int y = 0; y < gridHeight; ++y) {
            if (cells[x][y].hasMine()) {
                buttons[x][y]->setText("💣");
                if (!won) {
                    buttons[x][y]->setStyleSheet("background-color: red;");
                }
            }
        }
    }
    
    QString message = won ? "恭喜你赢了！" : "游戏结束！踩到地雷了！";
    QMessageBox::information(this, "游戏结束", message);
}