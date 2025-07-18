#ifndef QT_PROGRAMMING_2024_SHOTGUN_H
#define QT_PROGRAMMING_2024_SHOTGUN_H

#include "Weapon.h"

class Shotgun : public Weapon {
public:
    explicit Shotgun(QGraphicsItem *parent);
    
    // ��ȡ��������
    qreal getBulletSpeed() const { return bulletSpeed; }
    int getBulletDamage() const { return bulletDamage; }
    qint64 getFireRate() const { return fireRate; }
    
    // �����������ԣ����Զ��壩
    void setBulletSpeed(qreal speed) { bulletSpeed = speed; }
    void setBulletDamage(int damage) { bulletDamage = damage; }
    void setFireRate(qint64 rate) { fireRate = rate; }

private:
    qreal bulletSpeed = 60.0;    // �ӵ��ٶȣ����죩
    int bulletDamage = 30;       // �ӵ��˺������ߣ�
    qint64 fireRate = 2000;       // �����������룬������
};

#endif
