#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName, bool mirrorPixmap)
    : Item(parent, imagePath, mirrorPixmap), weaponName(weaponName) {
}

QString Weapon::getWeaponName() const {
    return weaponName;
}

void Weapon::mountToParent() {
    Mountable::mountToParent();
    // 设置武器在角色前方位置
    // 根据调试信息，武器应该在相对角色位置(33, -85)
    setPos(33, -85); // 在角色右前方
    setZValue(2); // 确保武器显示在最上层
}
