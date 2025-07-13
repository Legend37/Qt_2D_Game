#include "Pistol.h"

Pistol::Pistol(QGraphicsItem *parent) 
    : Weapon(parent, ":/Items/Weapon/pistol.png", "Pistol") {
    // 放大手枪到2倍大小
    setScale(2.0);
}

void Pistol::mountToParent() {
    Weapon::mountToParent();
    // 手枪特定的挂载位置和旋转
    setPos(20, -5); // 手枪位置稍微调整
    setRotation(-15); // 稍微倾斜，看起来更自然
}
