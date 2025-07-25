#include "Weapon.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

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

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // 调用基类的paint方法（虽然基类的paint是空的）
    Item::paint(painter, option, widget);
    
    // 只有在武器没有被拾取时才绘制白色边框
    if (pixmapItem && !isMounted()) {
        QRectF weaponRect = pixmapItem->boundingRect();
        
        // 设置白色画笔，线宽为2像素
        QPen whitePen(Qt::white, 2, Qt::SolidLine);
        painter->setPen(whitePen);
        painter->setBrush(Qt::NoBrush); // 不填充，只绘制边框
        
        // 绘制白色边框
        painter->drawRect(weaponRect);
    }
}
