#ifndef QT_PROGRAMMING_2024_SUBMACHINE_H
#define QT_PROGRAMMING_2024_SUBMACHINE_H

#include "Weapon.h"

class Submachine : public Weapon {
public:
    explicit Submachine(QGraphicsItem *parent);
    
    // ��ȡ��������
    qreal getBulletSpeed() const { return bulletSpeed; }
    int getBulletDamage() const { return bulletDamage; }
    qint64 getFireRate() const { return fireRate; }
    
    // �����������ԣ����Զ��壩
    void setBulletSpeed(qreal speed) { bulletSpeed = speed; }
    void setBulletDamage(int damage) { bulletDamage = damage; }
    void setFireRate(qint64 rate) { fireRate = rate; }

private:
    // ���ǹ���ԣ���������ֱ���޸��Զ��壩
    qreal bulletSpeed = 20.0;    // �ӵ��ٶȣ�������
    int bulletDamage = 10;        // �ӵ��˺����ϵͣ�
    qint64 fireRate = 50;       // �����������룬�ܿ죩
};

#endif
