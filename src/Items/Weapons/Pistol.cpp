#include "Pistol.h"

Pistol::Pistol(QGraphicsItem *parent)
    : Item(parent, ":/Items/Weapon/pistol.png", true),
      Weapon(parent, ":/Items/Weapon/pistol.png", "Pistol", true) {
    // 放大手枪到2倍大小
    setScale(2.0);
}

void Pistol::mountToParent() {
    Weapon::mountToParent();
    // 手枪在角色中心上方
    setPos(0, -35);
    setRotation(0); // 保持水平
}
