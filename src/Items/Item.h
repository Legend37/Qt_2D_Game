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

    // Gravity system
    virtual void advance(int step) override;
    void applyGravity(double deltaTime);
    bool isOnGround() const;
    
    // Set gravity state
    void setGravityEnabled(bool enabled) { gravityEnabled = enabled; }
    bool isGravityEnabled() const { return gravityEnabled; }

protected:
    QGraphicsPixmapItem *pixmapItem{};
    
    // Gravity related variables
    QPointF velocity{0, 0};
    bool gravityEnabled = false; // Gravity disabled by default, enabled by subclass
    static constexpr double gravity = 0.008; // Gravity acceleration, same as Character
    qint64 lastUpdateTime = 0;
};


#endif //QT_PROGRAMMING_2024_ITEM_H
