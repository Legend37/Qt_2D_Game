#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName, bool mirrorPixmap)
    : Item(parent, imagePath, mirrorPixmap), weaponName(weaponName) {
}

QString Weapon::getWeaponName() const {
    return weaponName;
}

void Weapon::mountToParent() {
    Mountable::mountToParent();
    // ���������ڽ�ɫ����λ���Ϸ�
    setPos(0, -30); // �ڽ�ɫ�����Ϸ�
    setZValue(2); // ȷ��������ʾ�����ϲ�
}
