#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName) 
    : Item(parent, imagePath), weaponName(weaponName) {
}

QString Weapon::getWeaponName() const {
    return weaponName;
}

void Weapon::mountToParent() {
    Mountable::mountToParent();
    // 设置武器在角色右手位置（相对位置）
    setPos(15, -10); // 武器在角色右侧稍微前方
    setZValue(1); // 确保武器显示在角色前面
}
