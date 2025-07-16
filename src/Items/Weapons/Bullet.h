#ifndef QT_PROGRAMMING_2024_BULLET_H
#define QT_PROGRAMMING_2024_BULLET_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPen>

class Character;

class Bullet : public QGraphicsEllipseItem {
public:
    Bullet(qreal x, qreal y, qreal vx, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(-10, -10, 20, 20, parent), velocity(vx, 0), shooter(nullptr) {
        setBrush(QBrush(QColor(255, 255, 180)));
        setPen(QPen(Qt::NoPen));
        setZValue(10);
        setPos(x, y); // ʹ�� setPos �����ӵ��ڳ����е�λ��
    }

    virtual ~Bullet();
    void advance(int phase) override;
    void checkCollisions();
    QPointF getSceneCenter() const;

    QPointF velocity;
    Character* shooter;
};

#endif
