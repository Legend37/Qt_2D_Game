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
    // �����ݵ�ͼ��
    QPixmap grassPixmap(":/Items/Maps/Battlefield/grass.png");
    
    if (grassPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load grass texture!";
        return;
    }
    
    // ��ȡ��ͼ�ı߽����
    QRectF mapRect = boundingRect();
    
    // ������һ���ݵ�Ԫ�أ�ʹ���������
    grass1 = new QGraphicsPixmapItem(grassPixmap, this);
    grass1->setPos(mapRect.width() * 0.23, mapRect.height() * 0.625); // ��Լ�ڵ�ͼ��23%��62.5%λ��
    grass1->setZValue(1); // ȷ���ݵ��ڱ���֮��
    
    // �����ڶ����ݵ�Ԫ�أ�ʹ���������
    grass2 = new QGraphicsPixmapItem(grassPixmap, this);
    grass2->setPos(mapRect.width() * 0.70, mapRect.height() * 0.625); // ��Լ�ڵ�ͼ��70%��62.5%λ��
    grass2->setZValue(1); // ȷ���ݵ��ڱ���֮��
    
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
    
    // ��ȡ��ͼ�ı߽����
    QRectF mapRect = boundingRect();
    
    // ��������Ԫ�أ�ʹ���������
    iceBlock = new QGraphicsPixmapItem(icePixmap, this);
    iceBlock->setPos(mapRect.width() * 0.45, mapRect.height() * 0.65); // ��Լ�ڵ�ͼ��39%��69%λ�� (����ƫ��)
    iceBlock->setZValue(2); // ȷ�������ڲݵ�֮��
    
    qDebug() << "[DEBUG] Ice block created at relative position";
    qDebug() << "[DEBUG] Map rect:" << mapRect;
    qDebug() << "[DEBUG] Ice block pos:" << iceBlock->pos();
}

qreal Battlefield::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return (sceneRect.top() + sceneRect.bottom()) * 0.63;
}
