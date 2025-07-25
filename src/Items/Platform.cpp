// Header file must be at the top, otherwise Platform type cannot be recognized
#include "Platform.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

// Static method: Find the nearest platform below the character
Platform* Platform::findNearestPlatformBelow(const QRectF& characterRect, const QList<Platform*>& platforms) {
    Platform* nearest = nullptr;
    qreal minY = std::numeric_limits<qreal>::max();
    qreal characterBottom = characterRect.bottom();
    qreal characterLeft = characterRect.left();
    qreal characterRight = characterRect.right();
    for (Platform* platform : platforms) {
        QRectF rect = platform->getPlatformRect();
        // Horizontally overlapped and platform is below the character
        if (characterRight > rect.left() && characterLeft < rect.right() && rect.top() > characterBottom) {
            if (rect.top() < minY) {
                minY = rect.top();
                nearest = platform;
            }
        }
    }
    return nearest;
}
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

Platform::Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : Item(parent, ""), 
      platformWidth(width), platformHeight(height) {
    
    // Set platform position
    setPos(x, y);
    
// Load rock texture
    QPixmap rockPixmap(":/Items/Maps/Battlefield/rock.png");
    if (rockPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load rock texture!";
        return;
    }
    
    // Calculate how many rock images are needed to fill the width
    qreal rockWidth = rockPixmap.width();
    qreal rockHeight = rockPixmap.height();
    
    // Calculate uniform scale ratio to match platform height
    qreal scale = height / rockHeight;
    qreal scaledRockWidth = rockWidth * scale;
    
    // 精确计算需要的瓦片数量，不添加额外瓦片
    int tilesNeeded = static_cast<int>(std::ceil(width / scaledRockWidth));
    
    // 创建多个岩石图片项来拼接
    for (int i = 0; i < tilesNeeded; ++i) {
        QGraphicsPixmapItem* rockTile = new QGraphicsPixmapItem(rockPixmap, this);
        qreal tileX = i * scaledRockWidth - 40; // 向左偏移40像素
        
        // 确保最后一个瓦片不会超出平台边界（基于原始宽度计算）
        if ((i * scaledRockWidth) + scaledRockWidth > width) {
            // 对最后一个瓦片进行裁剪
            qreal remainingWidth = width - (i * scaledRockWidth);
            QPixmap clippedPixmap = rockPixmap.copy(0, 0, remainingWidth / scale, rockPixmap.height());
            rockTile->setPixmap(clippedPixmap);
        }
        
        rockTile->setPos(tileX, 0); // 使用偏移后的位置
        rockTile->setScale(scale);
        rockTile->setZValue(0);
    }
}

bool Platform::isCharacterOnTop(const QRectF& characterRect) const {
    QRectF platformRect = getPlatformRect();
    
    // 从角色矩形获取角色的绝对位置（角色的中心点x坐标和底部y坐标）
    qreal characterX = characterRect.center().x();  // 角色中心x坐标
    qreal characterBottom = characterRect.bottom(); // 角色底部y坐标
    
    // 检查角色x坐标是否在平台范围内
    bool xInRange = characterX >= platformRect.left() && 
                    characterX <= platformRect.right();
    
    // 检查角色底部是否在平台顶部附近（允许小误差）
    qreal platformTop = platformRect.top();
    bool onTop = abs(characterBottom - platformTop) <= 1.0;
                
    return xInRange && onTop;
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
