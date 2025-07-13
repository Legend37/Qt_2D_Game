#ifndef QT_PROGRAMMING_2024_PLATFORM_H
#define QT_PROGRAMMING_2024_PLATFORM_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

class Platform : public QGraphicsRectItem {
public:
    Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    
    // 检查角色是否站在平台上
    bool isCharacterOnTop(const QRectF& characterRect) const;
    
    // 检查角色是否从下方撞击平台
    bool isCharacterHittingFromBelow(const QRectF& characterRect, qreal velocityY) const;
    
    // 获取平台顶部Y坐标
    qreal getTopY() const;
    
    // 获取平台底部Y坐标  
    qreal getBottomY() const;
};

#endif //QT_PROGRAMMING_2024_PLATFORM_H
