// 头文件必须在最顶部，否则 Platform 类型无法识别
#include "Platform.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

// 静态方法：查找角色下方最近的平台
Platform* Platform::findNearestPlatformBelow(const QRectF& characterRect, const QList<Platform*>& platforms) {
    Platform* nearest = nullptr;
    qreal minY = std::numeric_limits<qreal>::max();
    qreal characterBottom = characterRect.bottom();
    qreal characterLeft = characterRect.left();
    qreal characterRight = characterRect.right();
    qDebug() << "[DEBUG] 查找角色下方平台，角色rect:" << characterRect;
    for (Platform* platform : platforms) {
        QRectF rect = platform->getPlatformRect();
        qDebug() << "[DEBUG] 检查平台 rect:" << rect;
        // 水平重叠且平台在角色下方
        if (characterRight > rect.left() && characterLeft < rect.right() && rect.top() > characterBottom) {
            qDebug() << "[DEBUG] 平台在角色下方且水平重叠，平台top:" << rect.top();
            if (rect.top() < minY) {
                minY = rect.top();
                nearest = platform;
                qDebug() << "[DEBUG] 当前最近的平台top更新为:" << minY;
            }
        }
    }
    if (nearest) {
        qDebug() << "[DEBUG] 选中的最近平台top:" << minY << "rect:" << nearest->getPlatformRect();
    } else {
        qDebug() << "[DEBUG] 没有找到下方平台";
    }
    return nearest;
}
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
    
    // 计算统一缩放比例以匹配平台高度
    qreal scale = height / rockHeight;
    qreal scaledRockWidth = rockWidth * scale;
    
    // 精确计算需要的瓦片数量，不添加额外瓦片
    int tilesNeeded = static_cast<int>(std::ceil(width / scaledRockWidth));
    
    // 创建多个岩石图片项来拼接
    for (int i = 0; i < tilesNeeded; ++i) {
        QGraphicsPixmapItem* rockTile = new QGraphicsPixmapItem(rockPixmap, this);
        qreal tileX = i * scaledRockWidth;
        
        // 确保最后一个瓦片不会超出平台边界
        if (tileX + scaledRockWidth > width) {
            // 对最后一个瓦片进行裁剪
            qreal remainingWidth = width - tileX;
            QPixmap clippedPixmap = rockPixmap.copy(0, 0, remainingWidth / scale, rockPixmap.height());
            rockTile->setPixmap(clippedPixmap);
        }
        
        rockTile->setPos(tileX, 0);
        rockTile->setScale(scale);
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
