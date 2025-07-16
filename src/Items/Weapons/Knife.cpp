#include "Knife.h"

Knife::Knife(QGraphicsItem *parent) : Weapon(parent, ":/Items/Weapon/knife.png", "Knife") {
    // Knife ���ܷ����ӵ������Ե�ҩ��Ϊ0
    setAmmo(0);
    
    // �Ŵ�С����2����С������������һ��
    setScale(2.0);
}

void Knife::mountToParent() {
    Weapon::mountToParent();
    // С���ڽ�ɫ�����Ϸ�������������һ����λ��
    setPos(0, -30);
    setRotation(0); // ����ˮƽ
}
