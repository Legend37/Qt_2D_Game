#include "Knife.h"

Knife::Knife(QGraphicsItem *parent) : Weapon(parent, ":/Items/Weapon/knife.png", "Knife") {
    // Knife 不能发射子弹，所以弹药设为0
    setAmmo(0);
    
    // 放大小刀到2倍大小，和其他武器一样
    setScale(2.0);
}

void Knife::mountToParent() {
    Weapon::mountToParent();
    // 小刀在角色中心上方，和其他武器一样的位置
    setPos(0, -30);
    setRotation(0); // 保持水平
}
