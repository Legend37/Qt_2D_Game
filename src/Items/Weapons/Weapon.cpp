#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName) 
    : Item(parent, imagePath), weaponName(weaponName) {
}

QString Weapon::getWeaponName() const {
    return weaponName;
}

void Weapon::mountToParent() {
    Mountable::mountToParent();
    // ���������ڽ�ɫ����λ�ã����λ�ã�
    setPos(15, -10); // �����ڽ�ɫ�Ҳ���΢ǰ��
    setZValue(1); // ȷ��������ʾ�ڽ�ɫǰ��
}
