#ifndef CELL_H
#define CELL_H

#include <QWidget>

class Cell {
public:
    Cell();
    bool hasMine() const;
    void setMine(bool mine);
    bool isRevealed() const;
    void setRevealed(bool revealed);
    bool isFlagged() const;
    void setFlagged(bool flagged);
    int getNumber() const;
    void setNumber(int number);

private:
    bool mine;
    bool revealed;
    bool flagged;
    int number;
};

#endif // CELL_H