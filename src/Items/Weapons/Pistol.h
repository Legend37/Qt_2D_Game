#ifndef QT_PROGRAMMING_2024_PISTOL_H
#define QT_PROGRAMMING_2024_PISTOL_H

#include "Weapon.h"

class Pistol : public Weapon {
public:
    explicit Pistol(QGraphicsItem *parent);
    
    void mountToParent() override;
    
    // ��ȡ��������
    qreal getBulletSpeed() const { return bulletSpeed; }
    int getBulletDamage() const { return bulletDamage; }
    qint64 getFireRate() const { return fireRate; }
    
    // �����������ԣ����Զ��壩
    void setBulletSpeed(qreal speed) { bulletSpeed = speed; }
    void setBulletDamage(int damage) { bulletDamage = damage; }
    void setFireRate(qint64 rate) { fireRate = rate; }

private:
    // ��ǹ���ԣ���������ֱ���޸��Զ��壩
    qreal bulletSpeed = 22.5;    // �ӵ��ٶ�
    int bulletDamage = 15;       // �ӵ��˺�
    qint64 fireRate = 300;       // �����������룩
};

#endif
