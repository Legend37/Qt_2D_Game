//
// Created by gerw on 8/21/24.
//

#ifndef QT_PROGRAMMING_2024_FLAMEBREAKERARMOR_H
#define QT_PROGRAMMING_2024_FLAMEBREAKERARMOR_H


#include "Armor.h"

class FlamebreakerArmor : public Armor {
public:
    explicit FlamebreakerArmor(QGraphicsItem *parent = nullptr);
    float getFistDamageReduction() const override { return 1.0f; }
    float getKnifeDamageReduction() const override { return 0.5f; }
    
    QString getArmorType() const override { return "FlamebreakerArmor"; }
};


#endif //QT_PROGRAMMING_2024_FLAMEBREAKERARMOR_H
