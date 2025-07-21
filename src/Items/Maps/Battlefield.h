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
    
    // ����ɫ�Ƿ�վ���κ�ƽ̨�ϣ�������Ծƽ̨��
    bool isCharacterOnAnyPlatform(Character* character, qreal velocityY) const;

private:
    void setupGrassElements();
    void setupIceBlock();
    void setupPlatform();
    void setupJumpablePlatforms(); // ���ÿ���Ծƽ̨
    
    QGraphicsPixmapItem *grass1;
    QGraphicsPixmapItem *grass2;
    QGraphicsPixmapItem *iceBlock;
    Platform *groundPlatform; // ����ƽ̨
    QVector<Platform*> jumpablePlatforms; // ����Ծƽ̨�б�
};


#endif //QT_PROGRAMMING_2024_BATTLEFIELD_H
