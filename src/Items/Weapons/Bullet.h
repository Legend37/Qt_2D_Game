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
    Bullet(qreal x, qreal y, qreal vx, int damage = 20, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(-10, -10, 20, 20, parent), velocity(vx, 0), shooter(nullptr), damage(damage) {
        setBrush(QBrush(QColor(255, 255, 180)));
        setPen(QPen(Qt::NoPen));
        setZValue(10);
        setPos(x, y); // 使用 setPos 设置子弹在场景中的位置
    }

    virtual ~Bullet();
    void advance(int phase) override;
    void checkCollisions();
    QPointF getSceneCenter() const;
    
    // 获取和设置伤害
    int getDamage() const { return damage; }
    void setDamage(int dmg) { damage = dmg; }

    QPointF velocity;
    Character* shooter;

private:
    int damage; // 子弹伤害
};

#endif
