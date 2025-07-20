//
// Created by gerw on 8/21/24.
//

#include "Battlefield.h"
#include <QPixmap>
#include <QDebug>

Battlefield::Battlefield(QGraphicsItem *parent) : Map(parent, ":/Items/Maps/Battlefield/background.jpg") {
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
        qDebug() << "[ERROR] Failed to load ice block texture!";
        return;
    }
    
    // 获取地图的边界矩形
    QRectF mapRect = boundingRect();
    
    // 创建冰块元素，使用相对坐标
    iceBlock = new QGraphicsPixmapItem(icePixmap, this);
    iceBlock->setPos(mapRect.width() * 0.45, mapRect.height() * 0.65); // 大约在地图的39%和69%位置 (中央偏下)
    iceBlock->setZValue(2); // 确保冰块在草地之上
    
    qDebug() << "[DEBUG] Ice block created at relative position";
    qDebug() << "[DEBUG] Map rect:" << mapRect;
    qDebug() << "[DEBUG] Ice block pos:" << iceBlock->pos();
}

qreal Battlefield::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return (sceneRect.top() + sceneRect.bottom()) * 0.63;
}
