#include "Item.h"
#include "Maps/Battlefield.h"
#include <QDateTime>
#include <QDebug>

Item::Item(QGraphicsItem *parent, const QString &pixmapPath, bool mirrorPixmap) : QGraphicsItem(parent) {
    if (pixmapPath != "") {
        QPixmap pix(pixmapPath);
        if (mirrorPixmap) {
            pix = pix.transformed(QTransform().scale(-1, 1));
        }
        pixmapItem = new QGraphicsPixmapItem(pix, this);
    }
    lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
}

void Item::advance(int step) {
    if (step == 0) return; // 只在step=1时更新物理状态
    
    if (!gravityEnabled) return;
    
    // 计算时间差
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    double deltaTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // 应用重力
    applyGravity(deltaTime);
}

void Item::applyGravity(double deltaTime) {
    if (!gravityEnabled) return;
    
    bool onGround = isOnGround();
    
    if (!onGround) {
        // 应用重力加速度
        velocity.setY(velocity.y() + gravity * deltaTime);
        
        // 更新位置
        QPointF newPos = pos() + velocity;
        setPos(newPos);
        
        // 重新检查是否落地
        if (isOnGround()) {
            // 如果落地，停止垂直运动
            velocity.setY(0);
            
            // 调整位置到地面
            if (scene()) {
                auto items = scene()->items();
                for (auto item : items) {
                    if (auto battlefield = dynamic_cast<Battlefield*>(item)) {
                        qreal groundHeight = battlefield->getFloorHeight();
                        QPointF adjustedPos = pos();
                        adjustedPos.setY(groundHeight - boundingRect().height());
                        setPos(adjustedPos);
                        break;
                    }
                }
            }
        }
    }
}

bool Item::isOnGround() const {
    if (!scene()) return false;
    
    // 检查是否在地面或平台上
    auto items = scene()->items();
    for (auto item : items) {
        if (auto battlefield = dynamic_cast<Battlefield*>(item)) {
            qreal groundHeight = battlefield->getFloorHeight();
            QRectF itemRect = sceneBoundingRect();
            
            // 检查物品底部是否接触或低于地面
            if (itemRect.bottom() >= groundHeight) {
                return true;
            }
            
            // 还可以检查是否在跳跃平台上
            // 这里简化为只检查地面，如果需要可以扩展
        }
    }
    
    return false;
}
