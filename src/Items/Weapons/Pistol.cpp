#include "Pistol.h"

Pistol::Pistol(QGraphicsItem *parent) 
    : Weapon(parent, ":/Items/Weapon/pistol.png", "Pistol") {
    // �Ŵ���ǹ��2����С
    setScale(2.0);
}

void Pistol::mountToParent() {
    Weapon::mountToParent();
    // ��ǹ�ض��Ĺ���λ�ú���ת
    setPos(20, -5); // ��ǹλ����΢����
    setRotation(-15); // ��΢��б������������Ȼ
}
