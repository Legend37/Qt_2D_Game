#ifndef QT_PROGRAMMING_2024_HERO_H
#define QT_PROGRAMMING_2024_HERO_H

#include "Character.h"

class Hero : public Character {
public:
    explicit Hero(QGraphicsItem *parent = nullptr);
};

#endif
