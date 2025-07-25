#include "Medicine.h"
#include <QPainter>
#include <QPen>

Medicine::Medicine(QGraphicsItem *parent, const QString &pixmapPath, const QString &medicineName)
    : Mountable(parent, pixmapPath), medicineName(medicineName) {
    // 设置缩放，让药品和武器大小类似
    setScale(1.5);
}

void Medicine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // 调用基类的paint方法
    Item::paint(painter, option, widget);
    
    // 只有在药品没有被拾取时才绘制红色边框
    if (pixmapItem && !isMounted()) {
        QRectF medicineRect = pixmapItem->boundingRect();
        
        // 设置红色画笔，线宽为2像素
        QPen redPen(Qt::red, 2, Qt::SolidLine);
        painter->setPen(redPen);
        painter->setBrush(Qt::NoBrush); // 不填充，只绘制边框
        
        // 绘制红色边框
        painter->drawRect(medicineRect);
    }
}
