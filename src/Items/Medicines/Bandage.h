#ifndef QT_PROGRAMMING_2024_BANDAGE_H
#define QT_PROGRAMMING_2024_BANDAGE_H

#include "Medicine.h"

class Bandage : public Medicine {
public:
    explicit Bandage(QGraphicsItem *parent = nullptr);
    
    void applyEffect(class Character* character) override;
};

#endif // QT_PROGRAMMING_2024_BANDAGE_H
