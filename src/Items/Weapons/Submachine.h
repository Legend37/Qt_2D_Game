#ifndef QT_PROGRAMMING_2024_SUBMACHINE_H
#define QT_PROGRAMMING_2024_SUBMACHINE_H

#include "Weapon.h"

class Submachine : public Weapon {
public:
    explicit Submachine(QGraphicsItem *parent);
    
    // 获取武器属性
    qreal getBulletSpeed() const { return bulletSpeed; }
    int getBulletDamage() const { return bulletDamage; }
    qint64 getFireRate() const { return fireRate; }
    
    // 设置武器属性（可自定义）
    void setBulletSpeed(qreal speed) { bulletSpeed = speed; }
    void setBulletDamage(int damage) { bulletDamage = damage; }
    void setFireRate(qint64 rate) { fireRate = rate; }

private:
    // 冲锋枪属性（可在这里直接修改自定义）
    qreal bulletSpeed = 20.0;    // 子弹速度（较慢）
    int bulletDamage = 10;        // 子弹伤害（较低）
    qint64 fireRate = 50;       // 射击间隔（毫秒，很快）
};

#endif
