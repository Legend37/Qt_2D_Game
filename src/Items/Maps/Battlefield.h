//
// Created by gerw on 8/21/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLEFIELD_H
#define QT_PROGRAMMING_2024_BATTLEFIELD_H


#include "Map.h"
#include <QGraphicsPixmapItem>

class Platform;
class Character;

class Battlefield: public Map {
public:
    explicit Battlefield(QGraphicsItem *parent= nullptr);

    qreal getFloorHeight() override;
    
    // ����ɫ�Ƿ�վ�ڵ����ϣ�ƽ̨�ϣ�
    bool isCharacterOnGround(Character* character) const;

private:
    void setupGrassElements();
    void setupIceBlock();
    void setupPlatform();
    
    QGraphicsPixmapItem *grass1;
    QGraphicsPixmapItem *grass2;
    QGraphicsPixmapItem *iceBlock;
    Platform *groundPlatform; // ����ƽ̨
};


#endif //QT_PROGRAMMING_2024_BATTLEFIELD_H
