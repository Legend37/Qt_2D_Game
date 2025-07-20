#ifndef QT_PROGRAMMING_2024_ADRENALINE_H
#define QT_PROGRAMMING_2024_ADRENALINE_H

#include "Medicine.h"

class Adrenaline : public Medicine {
public:
    explicit Adrenaline(QGraphicsItem *parent = nullptr);
    
    void applyEffect(class Character* character) override;
};

#endif // QT_PROGRAMMING_2024_ADRENALINE_H
