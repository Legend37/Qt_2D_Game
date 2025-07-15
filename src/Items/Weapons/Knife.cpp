#include "Knife.h"

Knife::Knife(QGraphicsItem *parent) : Weapon(parent, ":/Items/Weapon/knife.png", "Knife") {
    // Knife ���ܷ����ӵ������Ե�ҩ��Ϊ0
    setAmmo(0);
    
    // ��С�����з�ת�ͷŴ������Ĳ���
    if (pixmapItem) {
        QTransform transform;
        transform.scale(-2.0, 2.0); // ˮƽ��ת���Ŵ�2��
        pixmapItem->setTransform(transform);
    }
}
