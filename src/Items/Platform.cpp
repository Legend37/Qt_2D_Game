#include "Platform.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

Platform::Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : Item(parent, ""), 
      platformWidth(width), platformHeight(height) {
    
    // 设置平台位置
    setPos(x, y);
    
// 加载岩石纹理
    QPixmap rockPixmap(":/Items/Maps/Battlefield/rock.png");
    if (rockPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load rock texture!";
        return;
    }
    
    // 计算需要多少个岩石图片来填满宽度
    qreal rockWidth = rockPixmap.width();
    qreal rockHeight = rockPixmap.height();
    int tilesNeeded = static_cast<int>(width / rockWidth) + 1; // +1确保完全覆盖
    
    // 计算统一缩放比例以匹配平台高度
    qreal scale = height / rockHeight;
    qreal scaledRockWidth = rockWidth * scale;
    
    // 创建多个岩石图片项来拼接
    for (int i = 0; i < tilesNeeded; ++i) {
        QGraphicsPixmapItem* rockTile = new QGraphicsPixmapItem(rockPixmap, this);
        rockTile->setPos(i * scaledRockWidth, 0); // 使用缩放后的宽度来排列
        rockTile->setScale(scale); // 统一缩放保持比例
        rockTile->setZValue(0);
    }
}

bool Platform::isCharacterOnTop(const QRectF& characterRect) const {
    QRectF platformRect = getPlatformRect();
    
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
    
    QRectF platformRect = getPlatformRect();
    
    // 检查水平重叠
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // 检查角色头部是否撞击平台底部
    bool hittingBottom = characterRect.top() <= platformRect.bottom() + 5 && 
                        characterRect.top() >= platformRect.bottom() - 10;
                        
    return horizontalOverlap && hittingBottom;
}

qreal Platform::getTopY() const {
    return pos().y();
}

qreal Platform::getBottomY() const {
    return pos().y() + platformHeight;
}

QRectF Platform::getPlatformRect() const {
    return QRectF(pos().x(), pos().y(), platformWidth, platformHeight);
}
