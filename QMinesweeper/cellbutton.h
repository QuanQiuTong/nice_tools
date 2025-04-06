#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class CellButton : public QPushButton {
    Q_OBJECT

public:
    CellButton(int x, int y, QWidget* parent = nullptr);
    
    int getX() const { return x; }
    int getY() const { return y; }
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    
signals:
    void rightClicked();
    void doubleClicked();
    
private:
    int x, y;
};
    
#endif // CELLBUTTON_H