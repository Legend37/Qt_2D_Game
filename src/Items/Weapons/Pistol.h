#ifndef QT_PROGRAMMING_2024_PISTOL_H
#define QT_PROGRAMMING_2024_PISTOL_H

#include "Weapon.h"

class Pistol : public Weapon {
public:
    explicit Pistol(QGraphicsItem *parent);
    
    void mountToParent() override;
};

#endif
