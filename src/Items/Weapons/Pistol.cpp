#include "Pistol.h"

Pistol::Pistol(QGraphicsItem *parent)
    : Item(parent, ":/Items/Weapon/pistol.png", true),
      Weapon(parent, ":/Items/Weapon/pistol.png", "Pistol", true) {
    // 放大手枪到2倍大小
    setScale(2.0);
    // 设置手枪的子弹数量
    setAmmo(maxAmmo);
}

void Pistol::mountToParent() {
    Weapon::mountToParent();
    // 手枪在角色右前方
    setPos(33, -85);
    setRotation(0); // 保持水平
}
