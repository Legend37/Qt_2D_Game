//
// Created by gerw on 8/21/24.
//

#ifndef QT_PROGRAMMING_2024_FLAMEBREAKERARMOR_H
#define QT_PROGRAMMING_2024_FLAMEBREAKERARMOR_H


#include "Armor.h"

class FlamebreakerArmor : public Armor {
public:
    explicit FlamebreakerArmor(QGraphicsItem *parent = nullptr);
    
    // ∑¿ª§–ßπ˚÷ÿ–¥
    float getFistDamageReduction() const override { return 1.0f; } // 100%»≠Õ∑…À∫¶ºı√‚
    float getKnifeDamageReduction() const override { return 0.5f; } // 50%–°µ∂…À∫¶ºı√‚
    
    QString getArmorType() const override { return "FlamebreakerArmor"; }
};


#endif //QT_PROGRAMMING_2024_FLAMEBREAKERARMOR_H
