#include "cellbutton.h"
#include <QPushButton>
#include <QMouseEvent>

CellButton::CellButton(int x, int y, QWidget* parent)
    : QPushButton(parent), x(x), y(y) {
    setMouseTracking(true);
}

void CellButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        emit rightClicked();
    } else {
        QPushButton::mousePressEvent(event);
    }
}

void CellButton::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked();
    } else {
        QPushButton::mouseDoubleClickEvent(event);
    }
}