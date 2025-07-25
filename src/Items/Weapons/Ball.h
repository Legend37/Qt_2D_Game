#ifndef QT_PROGRAMMING_2024_BALL_H
#define QT_PROGRAMMING_2024_BALL_H

#include "Weapon.h"

class Ball : public Weapon {
public:
    explicit Ball(QGraphicsItem *parent = nullptr);
    
    void mountToParent() override;
    void advance(int phase) override;
    
    QString getWeaponName() const { return "Ball"; }
    
    QPointF getSceneCenter() const;
    
    void checkCollisions();
    
    void setVelocity(qreal vx, qreal vy);
    
    void setThrownMode(bool thrown = true);
    
    void unmount() override;
    
    int getMaxAmmo() const { return maxAmmo; }
    void setMaxAmmo(int ammo) { maxAmmo = ammo; setAmmo(ammo); }
    
    QGraphicsItem* shooter = nullptr;
    
    bool isMounted() const;
    
    void deleteLater();
private:
    QPointF ballVelocity; 
    static constexpr qreal thrownGravity = 1.6;
    qint64 creationTime;
    qint64 groundTimer = 0;
    static constexpr qint64 maxLifetime = 60000;
    bool active = true;
    bool isThrown = false;
    bool hasBeenPickedUp = false;
    int maxAmmo = 1;
};

#endif
