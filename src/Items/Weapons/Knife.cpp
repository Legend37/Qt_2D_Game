#include "Knife.h"

Knife::Knife(QGraphicsItem *parent) 
    : Item(parent, ":/Items/Weapon/knife.png", false),
      Weapon(parent, ":/Items/Weapon/knife.png", "Knife") {
    // Knife 不能发射子弹，所以弹药设为0
    setAmmo(0);
    
    // 放大小刀到2倍大小，和其他武器一样
    setScale(2.0);
}

void Knife::mountToParent() {
    Weapon::mountToParent();
    // 小刀在角色右前方，和其他武器一样的位置
    setPos(33, -85);
    setRotation(0); // 保持水平
}
