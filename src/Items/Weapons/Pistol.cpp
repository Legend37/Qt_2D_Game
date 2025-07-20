#include "Pistol.h"

Pistol::Pistol(QGraphicsItem *parent)
    : Item(parent, ":/Items/Weapon/pistol.png", true),
      Weapon(parent, ":/Items/Weapon/pistol.png", "Pistol", true) {
    // �Ŵ���ǹ��2����С
    setScale(2.0);
}

void Pistol::mountToParent() {
    Weapon::mountToParent();
    // ��ǹ�ڽ�ɫ�����Ϸ�
    setPos(0, -35);
    setRotation(0); // ����ˮƽ
}
