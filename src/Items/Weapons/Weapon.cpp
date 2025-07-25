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
    // ���������ڽ�ɫǰ��λ��
    // ���ݵ�����Ϣ������Ӧ������Խ�ɫλ��(33, -85)
    setPos(33, -85); // �ڽ�ɫ��ǰ��
    setZValue(2); // ȷ��������ʾ�����ϲ�
}

void Weapon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // ���û����paint��������Ȼ�����paint�ǿյģ�
    Item::paint(painter, option, widget);
    
    // ֻ��������û�б�ʰȡʱ�Ż��ư�ɫ�߿�
    if (pixmapItem && !isMounted()) {
        QRectF weaponRect = pixmapItem->boundingRect();
        
        // ���ð�ɫ���ʣ��߿�Ϊ2����
        QPen whitePen(Qt::white, 2, Qt::SolidLine);
        painter->setPen(whitePen);
        painter->setBrush(Qt::NoBrush); // ����䣬ֻ���Ʊ߿�
        
        // ���ư�ɫ�߿�
        painter->drawRect(weaponRect);
    }
}
