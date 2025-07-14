#ifndef QT_PROGRAMMING_2024_BULLET_H
#define QT_PROGRAMMING_2024_BULLET_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QColor>
#include <QDebug>

class Bullet : public QGraphicsEllipseItem {
public:
    Bullet(qreal x, qreal y, qreal vx, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(x - 10, y - 10, 20, 20, parent), velocity(vx, 0) {
        setBrush(QBrush(QColor(255, 255, 180)));
        setPen(Qt::NoPen);
        setZValue(10);
    }

    void advance(int phase) override {
        if (phase == 0) return;
        qDebug() << "[DEBUG] Bullet advancing, current pos:" << pos() << "velocity:" << velocity;
        moveBy(velocity.x(), velocity.y());
        qDebug() << "[DEBUG] Bullet new pos:" << pos();
        
        // Remove bullet if it goes off screen
        if (pos().x() < -400 || pos().x() > 400 || pos().y() < -400 || pos().y() > 400) {
            qDebug() << "[DEBUG] Bullet removed (off screen)";
            scene()->removeItem(this);
            delete this;
        }
    }

    QPointF velocity;
};

#endif
