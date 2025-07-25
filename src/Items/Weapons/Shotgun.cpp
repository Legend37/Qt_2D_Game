#include "Shotgun.h"

Shotgun::Shotgun(QGraphicsItem *parent)
    : Item(parent, ":/Items/Weapon/shotgun.png", true),
      Weapon(parent, ":/Items/Weapon/shotgun.png", "Shotgun", true) {
    setScale(2.0);
    // 设置霰弹枪的子弹数量
    setAmmo(maxAmmo);
}
