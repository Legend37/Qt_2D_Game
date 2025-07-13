#include "Platform.h"

Platform::Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent) {
    
    // 设置平台外观
    setBrush(QColor(139, 69, 19)); // 棕色平台
    setPen(QPen(QColor(101, 67, 33), 2)); // 深棕色边框
}

bool Platform::isCharacterOnTop(const QRectF& characterRect) const {
    QRectF platformRect = rect().translated(pos());
    
    // 检查水平重叠
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // 检查角色底部是否在平台顶部附近
    qreal characterBottom = characterRect.bottom();
    qreal platformTop = platformRect.top();
    
    bool onTop = characterBottom >= platformTop - 2 && 
                characterBottom <= platformTop + 5;
                
    return horizontalOverlap && onTop;
}

bool Platform::isCharacterHittingFromBelow(const QRectF& characterRect, qreal velocityY) const {
    if (velocityY >= 0) return false; // 只检查向上运动
    
    QRectF platformRect = rect().translated(pos());
    
    // 检查水平重叠
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // 检查角色头部是否撞击平台底部
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
