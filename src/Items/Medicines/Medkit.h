#ifndef QT_PROGRAMMING_2024_MEDKIT_H
#define QT_PROGRAMMING_2024_MEDKIT_H

#include "Medicine.h"

class Medkit : public Medicine {
public:
    explicit Medkit(QGraphicsItem *parent = nullptr);
    
    void applyEffect(class Character* character) override;
};

#endif // QT_PROGRAMMING_2024_MEDKIT_H
