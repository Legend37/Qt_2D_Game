#ifndef QT_PROGRAMMING_2024_PISTOL_H
#define QT_PROGRAMMING_2024_PISTOL_H

#include "Weapon.h"

class Pistol : public Weapon {
public:
    explicit Pistol(QGraphicsItem *parent);
    
    void mountToParent() override;
    
    // 获取武器属性
    qreal getBulletSpeed() const { return bulletSpeed; }
    int getBulletDamage() const { return bulletDamage; }
    qint64 getFireRate() const { return fireRate; }
    
    // 设置武器属性（可自定义）
    void setBulletSpeed(qreal speed) { bulletSpeed = speed; }
    void setBulletDamage(int damage) { bulletDamage = damage; }
    void setFireRate(qint64 rate) { fireRate = rate; }

private:
    // 手枪属性（可在这里直接修改自定义）
    qreal bulletSpeed = 22.5;    // 子弹速度
    int bulletDamage = 15;       // 子弹伤害
    qint64 fireRate = 300;       // 射击间隔（毫秒）
};

#endif
