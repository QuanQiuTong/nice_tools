#include "cell.h"

Cell::Cell() 
    : mine(false), revealed(false), flagged(false), number(0) 
{
}

bool Cell::hasMine() const {
    return mine;
}

void Cell::setMine(bool mine) {
    this->mine = mine;
}

bool Cell::isRevealed() const {
    return revealed;
}

void Cell::setRevealed(bool revealed) {
    this->revealed = revealed;
}

bool Cell::isFlagged() const {
    return flagged;
}

void Cell::setFlagged(bool flagged) {
    this->flagged = flagged;
}

int Cell::getNumber() const {
    return number;
}

void Cell::setNumber(int number) {
    this->number = number;
}