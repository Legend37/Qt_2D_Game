#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName, bool mirrorPixmap)
    : Item(parent, imagePath, mirrorPixmap), weaponName(weaponName) {
}

QString Weapon::getWeaponName() const {
    return weaponName;
}

void Weapon::mountToParent() {
    Mountable::mountToParent();
    // 设置武器在角色中心位置上方
    setPos(0, -30); // 在角色中心上方
    setZValue(2); // 确保武器显示在最上层
}
