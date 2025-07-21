#include "Knife.h"

Knife::Knife(QGraphicsItem *parent) 
    : Item(parent, ":/Items/Weapon/knife.png", false),
      Weapon(parent, ":/Items/Weapon/knife.png", "Knife") {
    // Knife ���ܷ����ӵ������Ե�ҩ��Ϊ0
    setAmmo(0);
    
    // �Ŵ�С����2����С������������һ��
    setScale(2.0);
}

void Knife::mountToParent() {
    Weapon::mountToParent();
    // С���ڽ�ɫ��ǰ��������������һ����λ��
    setPos(33, -85);
    setRotation(0); // ����ˮƽ
}
