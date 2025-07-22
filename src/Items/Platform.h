#ifndef QT_PROGRAMMING_2024_PLATFORM_H
#define QT_PROGRAMMING_2024_PLATFORM_H

#include "Item.h"
#include <QRectF>

class Platform : public Item {
public:
    Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

    // ����ɫ�Ƿ�վ��ƽ̨��
    bool isCharacterOnTop(const QRectF& characterRect) const;

    // ����ɫ�Ƿ���·�ײ��ƽ̨
    bool isCharacterHittingFromBelow(const QRectF& characterRect, qreal velocityY) const;

    // ��ȡƽ̨����Y����
    qreal getTopY() const;

    // ��ȡƽ̨�ײ�Y����  
    qreal getBottomY() const;

    // ��ȡƽ̨����
    QRectF getPlatformRect() const;

    // ���ҽ�ɫ�·������ƽ̨
    static Platform* findNearestPlatformBelow(const QRectF& characterRect, const QList<Platform*>& platforms);

private:
    qreal platformWidth;
    qreal platformHeight;
};
#endif //QT_PROGRAMMING_2024_PLATFORM_H
