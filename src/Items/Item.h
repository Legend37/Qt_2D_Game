//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_ITEM_H
#define QT_PROGRAMMING_2024_ITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>

class Item : public QGraphicsItem {
public:
    explicit Item(QGraphicsItem *parent, const QString &pixmapPath, bool mirrorPixmap = false);

    [[nodiscard]] QRectF boundingRect() const override {
        if (pixmapItem != nullptr) {
            return this->pixmapItem->boundingRect();
        }
        // Empty rectangle makes the empty item not selectable and not collider
        return {};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override {
        // Draw nothing as it is an empty item
        // Its pixmapItem is automatically handled by children's paint()
    }

    // 重力系统
    virtual void advance(int step) override;
    void applyGravity(double deltaTime);
    bool isOnGround() const;
    
    // 设置重力状态
    void setGravityEnabled(bool enabled) { gravityEnabled = enabled; }
    bool isGravityEnabled() const { return gravityEnabled; }

protected:
    QGraphicsPixmapItem *pixmapItem{};
    
    // 重力相关变量
    QPointF velocity{0, 0};
    bool gravityEnabled = false; // 默认关闭重力，由子类决定是否启用
    static constexpr double gravity = 0.008; // 与Character保持一致的重力加速度
    qint64 lastUpdateTime = 0;
};


#endif //QT_PROGRAMMING_2024_ITEM_H
