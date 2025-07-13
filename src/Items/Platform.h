#ifndef QT_PROGRAMMING_2024_PLATFORM_H
#define QT_PROGRAMMING_2024_PLATFORM_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

class Platform : public QGraphicsRectItem {
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
};

#endif //QT_PROGRAMMING_2024_PLATFORM_H
