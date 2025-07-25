#include "Submachine.h"

Submachine::Submachine(QGraphicsItem *parent)
    : Item(parent, ":/Items/Weapon/submachine.png", true),
      Weapon(parent, ":/Items/Weapon/submachine.png", "Submachine", true) {
    setScale(2.0);
    // 设置冲锋枪的子弹数量
    setAmmo(maxAmmo);
}
