//
// Created by gerw on 8/21/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLEFIELD_H
#define QT_PROGRAMMING_2024_BATTLEFIELD_H


#include "Map.h"
#include <QGraphicsPixmapItem>

class Battlefield: public Map {
public:
    explicit Battlefield(QGraphicsItem *parent= nullptr);

    qreal getFloorHeight() override;

private:
    void setupGrassElements();
    void setupIceBlock();
    QGraphicsPixmapItem *grass1;
    QGraphicsPixmapItem *grass2;
    QGraphicsPixmapItem *iceBlock;
};


#endif //QT_PROGRAMMING_2024_BATTLEFIELD_H
