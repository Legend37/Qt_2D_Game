#include "Platform.h"

Platform::Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent) {
    
    // ����ƽ̨���
    setBrush(QColor(139, 69, 19)); // ��ɫƽ̨
    setPen(QPen(QColor(101, 67, 33), 2)); // ����ɫ�߿�
}

bool Platform::isCharacterOnTop(const QRectF& characterRect) const {
    QRectF platformRect = rect().translated(pos());
    
    // ���ˮƽ�ص�
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // ����ɫ�ײ��Ƿ���ƽ̨��������
    qreal characterBottom = characterRect.bottom();
    qreal platformTop = platformRect.top();
    
    bool onTop = characterBottom >= platformTop - 2 && 
                characterBottom <= platformTop + 5;
                
    return horizontalOverlap && onTop;
}

bool Platform::isCharacterHittingFromBelow(const QRectF& characterRect, qreal velocityY) const {
    if (velocityY >= 0) return false; // ֻ��������˶�
    
    QRectF platformRect = rect().translated(pos());
    
    // ���ˮƽ�ص�
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // ����ɫͷ���Ƿ�ײ��ƽ̨�ײ�
    bool hittingBottom = characterRect.top() <= platformRect.bottom() + 5 && 
                        characterRect.top() >= platformRect.bottom() - 10;
                        
    return horizontalOverlap && hittingBottom;
}

qreal Platform::getTopY() const {
    return pos().y() + rect().top();
}

qreal Platform::getBottomY() const {
    return pos().y() + rect().bottom();
}
