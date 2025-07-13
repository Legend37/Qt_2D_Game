#include "Shotgun.h"

Shotgun::Shotgun(QGraphicsItem *parent) 
    : Weapon(parent, ":/Items/Weapon/shotgun.png", "Shotgun", true) {
    setScale(2.0);
}
