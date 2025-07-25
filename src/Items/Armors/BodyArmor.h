//
// Created for Qt Programming 2025
//

#ifndef QT_PROGRAMMING_2025_BODYARMOR_H
#define QT_PROGRAMMING_2025_BODYARMOR_H

#include "Armor.h"

class BodyArmor : public Armor {
public:
    explicit BodyArmor(QGraphicsItem *parent = nullptr);
    
    // 防护效果重写
    float getBulletDamageReduction() const override { return 0.3f; } // 30%子弹伤害减免
    
    QString getArmorType() const override { return "BodyArmor"; }
};

#endif //QT_PROGRAMMING_2025_BODYARMOR_H
