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
    
    // 检查角色是否站在地面上（平台上）
    bool isCharacterOnGround(Character* character) const;
    
    // 检查角色是否站在任何平台上（包括跳跃平台）
    bool isCharacterOnAnyPlatform(Character* character, qreal velocityY) const;

private:
    void setupGrassElements();
    void setupIceBlock();
    void setupPlatform();
    void setupJumpablePlatforms(); // 设置可跳跃平台
    
    QGraphicsPixmapItem *iceBlock;
    Platform *groundPlatform; // 地面平台
    QVector<Platform*> jumpablePlatforms; // 可跳跃平台列表
};


#endif //QT_PROGRAMMING_2024_BATTLEFIELD_H
