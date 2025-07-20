//
// Created by gerw on 8/21/24.
//

#include "Battlefield.h"
#include "../Platform.h"
#include "../Characters/Character.h"
#include <QPixmap>
#include <QDebug>

Battlefield::Battlefield(QGraphicsItem *parent) : Map(parent, ":/Items/Maps/Battlefield/background.jpg") {
    setupPlatform();
    setupGrassElements();
    setupIceBlock();
}

void Battlefield::setupGrassElements() {
    // 创建草地图像
    QPixmap grassPixmap(":/Items/Maps/Battlefield/grass.png");
    
    if (grassPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load grass texture!";
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
    
    qDebug() << "[DEBUG] Grass elements created at relative positions";
    qDebug() << "[DEBUG] Map rect:" << mapRect;
    qDebug() << "[DEBUG] Grass1 pos:" << grass1->pos();
    qDebug() << "[DEBUG] Grass2 pos:" << grass2->pos();
}

void Battlefield::setupIceBlock() {
    QPixmap icePixmap(":/Items/Maps/Battlefield/Ice_block.png");
    if (icePixmap.isNull()) {
        // qDebug() << "[ERROR] Failed to load ice block texture!";
        return;
    }
    
    // 获取地图的边界矩形
    QRectF mapRect = boundingRect();
    
    // 创建冰块元素，使用相对坐标
    iceBlock = new QGraphicsPixmapItem(icePixmap, this);
    iceBlock->setZValue(2); // 确保冰块在草地之上
    
    // qDebug() << "[DEBUG] Ice block created at relative position";
    // qDebug() << "[DEBUG] Map rect:" << mapRect;
    // qDebug() << "[DEBUG] Ice block pos:" << iceBlock->pos();
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
