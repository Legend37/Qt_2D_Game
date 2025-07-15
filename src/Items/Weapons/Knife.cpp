#include "Knife.h"

Knife::Knife(QGraphicsItem *parent) : Weapon(parent, ":/Items/Weapon/knife.png", "Knife") {
    // Knife 不能发射子弹，所以弹药设为0
    setAmmo(0);
    
    // 对小刀进行翻转和放大两倍的操作
    if (pixmapItem) {
        QTransform transform;
        transform.scale(-2.0, 2.0); // 水平翻转并放大2倍
        pixmapItem->setTransform(transform);
    }
}
