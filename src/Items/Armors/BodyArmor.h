//
// Created for Qt Programming 2025
//

#ifndef QT_PROGRAMMING_2025_BODYARMOR_H
#define QT_PROGRAMMING_2025_BODYARMOR_H

#include "Armor.h"

class BodyArmor : public Armor {
public:
    explicit BodyArmor(QGraphicsItem *parent = nullptr);
    
    // ����Ч����д
    float getBulletDamageReduction() const override { return 0.3f; } // 30%�ӵ��˺�����
    
    QString getArmorType() const override { return "BodyArmor"; }
};

#endif //QT_PROGRAMMING_2025_BODYARMOR_H
