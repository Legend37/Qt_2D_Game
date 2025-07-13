#include "Submachine.h"

Submachine::Submachine(QGraphicsItem *parent) 
    : Weapon(parent, ":/Items/Weapon/submachine.png", "Submachine", true) {
    setScale(2.0);
}
