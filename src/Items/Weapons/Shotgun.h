#ifndef QT_PROGRAMMING_2024_SHOTGUN_H
#define QT_PROGRAMMING_2024_SHOTGUN_H

#include "Weapon.h"

class Shotgun : public Weapon {
public:
    explicit Shotgun(QGraphicsItem *parent);
    
    // 获取武器属性
    qreal getBulletSpeed() const { return bulletSpeed; }
    int getBulletDamage() const { return bulletDamage; }
    qint64 getFireRate() const { return fireRate; }
    
    // 设置武器属性（可自定义）
    void setBulletSpeed(qreal speed) { bulletSpeed = speed; }
    void setBulletDamage(int damage) { bulletDamage = damage; }
    void setFireRate(qint64 rate) { fireRate = rate; }
    
    // 获取和设置子弹数量
    int getMaxAmmo() const { return maxAmmo; }
    void setMaxAmmo(int ammo) { maxAmmo = ammo; setAmmo(ammo); }

private:
    qreal bulletSpeed = 50.0;    // 子弹速度（更快）
    int bulletDamage = 60;       // 子弹伤害（更高）
    qint64 fireRate = 3000;       // 射击间隔（毫秒，更慢）
    int maxAmmo = 6;              // 霰弹枪默认6发子弹
};

#endif
