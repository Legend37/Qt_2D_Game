#include "Weapon.h"

Weapon::Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName, bool mirrorPixmap)
    : Item(parent, imagePath, mirrorPixmap), weaponName(weaponName) {
}

QString Weapon::getWeaponName() const {
    return weaponName;
}

void Weapon::mountToParent() {
    Mountable::mountToParent();
    // ���������ڽ�ɫǰ��λ��
    // ���ݵ�����Ϣ������Ӧ������Խ�ɫλ��(33, -85)
    setPos(33, -85); // �ڽ�ɫ��ǰ��
    setZValue(2); // ȷ��������ʾ�����ϲ�
}
