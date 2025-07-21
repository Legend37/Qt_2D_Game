//
// Created by gerw on 8/21/24.
//

#include "Battlefield.h"
#include "../Platform.h"
#include "../Characters/Character.h"
#include <QPixmap>
#include <QDebug>
#include <QVector>

Battlefield::Battlefield(QGraphicsItem *parent) : Map(parent, ":/Items/Maps/Battlefield/background.jpg") {
    setupPlatform();
    setupJumpablePlatforms();
    setupGrassElements();
    setupIceBlock();
}

void Battlefield::setupGrassElements() {
    // 创建草地图像
    QPixmap grassPixmap(":/Items/Maps/Battlefield/grass.png");
    
    if (grassPixmap.isNull()) {
        return;
    }
    
    // 获取地图的边界矩形
    QRectF mapRect = boundingRect();
    
    // 创建第一个草地元素，使用相对坐标
    grass1 = new QGraphicsPixmapItem(grassPixmap, this);
    grass1->setPos(mapRect.width() * 0.23, mapRect.height() * 0.625); // 大约在地图的23%和62.5%位置
    grass1->setZValue(1); // 确保草地在背景之上
    
    // 创建第二个草地元素，使用相对坐标
    grass2 = new QGraphicsPixmapItem(grassPixmap, this);
    grass2->setPos(mapRect.width() * 0.70, mapRect.height() * 0.625); // 大约在地图的70%和62.5%位置
    grass2->setZValue(1); // 确保草地在背景之上
}

void Battlefield::setupIceBlock() {
    QPixmap icePixmap(":/Items/Maps/Battlefield/Ice_block.png");
    if (icePixmap.isNull()) {
        return;
    }
    
    // 获取地图的边界矩形
    QRectF mapRect = boundingRect();
    
    // 创建冰块元素，使用相对坐标
    iceBlock = new QGraphicsPixmapItem(icePixmap, this);
    iceBlock->setPos(mapRect.width() * 0.45, mapRect.height() * 0.65); // 大约在地图的39%和69%位置 (中央偏下)
    iceBlock->setZValue(2); // 确保冰块在草地之上
}


void Battlefield::setupPlatform() {
    // 获取场景边界来确定平台尺寸
    QRectF sceneRect = QRectF(0, 0, 1280, 720); // 默认场景大小
    
    // 创建横跨整个场景宽度的岩石平台，y坐标为600
    qreal platformY = 600;
    qreal platformWidth = sceneRect.width();
    qreal platformHeight = 40; // 平台厚度
    
    groundPlatform = new Platform(0, platformY, platformWidth, platformHeight, this);
    
}

void Battlefield::setupJumpablePlatforms() {
    // 添加第一个可跳跃平台：y=400, x=200到x=300
    Platform* jumpPlatform1 = new Platform(200, 400, 100, 30, this);
    jumpablePlatforms.append(jumpPlatform1);
}

qreal Battlefield::getFloorHeight() {
    // 地面高度现在是平台顶部
    return 600;
}

bool Battlefield::isCharacterOnGround(Character* character) const {
    if (!character || !groundPlatform) return false;
    
    // 获取角色的碰撞箱
    QRectF characterRect = character->getHitBox();
    
    // 检查角色是否站在平台上
    return groundPlatform->isCharacterOnTop(characterRect);
}

bool Battlefield::isCharacterOnAnyPlatform(Character* character, qreal velocityY) const {
    if (!character) return false;
    
    // 获取角色的碰撞箱
    QRectF characterRect = character->getHitBox();
    
    // 只在下降时检查平台碰撞
    if (velocityY <= 0) return false;
    
    // 检查地面平台
    if (groundPlatform && groundPlatform->isCharacterOnTop(characterRect)) {
        return true;
    }
    
    // 检查所有可跳跃平台
    for (Platform* platform : jumpablePlatforms) {
        if (platform && platform->isCharacterOnTop(characterRect)) {
            return true;
        }
    }
    
    return false;
}
