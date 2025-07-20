#include "Shotgun.h"

Shotgun::Shotgun(QGraphicsItem *parent)
    : Item(parent, ":/Items/Weapon/shotgun.png", true),
      Weapon(parent, ":/Items/Weapon/shotgun.png", "Shotgun", true) {
    setScale(2.0);
}
