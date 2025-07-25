#include "Medicine.h"
#include <QPainter>
#include <QPen>

Medicine::Medicine(QGraphicsItem *parent, const QString &pixmapPath, const QString &medicineName)
    : Mountable(parent, pixmapPath), medicineName(medicineName) {
    // �������ţ���ҩƷ��������С����
    setScale(1.5);
}

void Medicine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // ���û����paint����
    Item::paint(painter, option, widget);
    
    // ֻ����ҩƷû�б�ʰȡʱ�Ż��ƺ�ɫ�߿�
    if (pixmapItem && !isMounted()) {
        QRectF medicineRect = pixmapItem->boundingRect();
        
        // ���ú�ɫ���ʣ��߿�Ϊ2����
        QPen redPen(Qt::red, 2, Qt::SolidLine);
        painter->setPen(redPen);
        painter->setBrush(Qt::NoBrush); // ����䣬ֻ���Ʊ߿�
        
        // ���ƺ�ɫ�߿�
        painter->drawRect(medicineRect);
    }
}
