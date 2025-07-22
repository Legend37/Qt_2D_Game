#ifndef QT_PROGRAMMING_2024_PLATFORM_H
#define QT_PROGRAMMING_2024_PLATFORM_H

#include "Item.h"
#include <QRectF>

class Platform : public Item {
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

    // 获取平台矩形
    QRectF getPlatformRect() const;

    // 查找角色下方最近的平台
    static Platform* findNearestPlatformBelow(const QRectF& characterRect, const QList<Platform*>& platforms);

private:
    qreal platformWidth;
    qreal platformHeight;
};
#endif //QT_PROGRAMMING_2024_PLATFORM_H
