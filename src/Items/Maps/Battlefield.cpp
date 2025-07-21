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
    // �����ݵ�ͼ��
    QPixmap grassPixmap(":/Items/Maps/Battlefield/grass.png");
    
    if (grassPixmap.isNull()) {
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
}

void Battlefield::setupIceBlock() {
    QPixmap icePixmap(":/Items/Maps/Battlefield/Ice_block.png");
    if (icePixmap.isNull()) {
        return;
    }
    
    // ��ȡ��ͼ�ı߽����
    QRectF mapRect = boundingRect();
    
    // ��������Ԫ�أ�ʹ���������
    iceBlock = new QGraphicsPixmapItem(icePixmap, this);
    iceBlock->setPos(mapRect.width() * 0.45, mapRect.height() * 0.65); // ��Լ�ڵ�ͼ��39%��69%λ�� (����ƫ��)
    iceBlock->setZValue(2); // ȷ�������ڲݵ�֮��
}


void Battlefield::setupPlatform() {
    // ��ȡ�����߽���ȷ��ƽ̨�ߴ�
    QRectF sceneRect = QRectF(0, 0, 1280, 720); // Ĭ�ϳ�����С
    
    // �����������������ȵ���ʯƽ̨��y����Ϊ600
    qreal platformY = 600;
    qreal platformWidth = sceneRect.width();
    qreal platformHeight = 40; // ƽ̨���
    
    groundPlatform = new Platform(0, platformY, platformWidth, platformHeight, this);
    
}

void Battlefield::setupJumpablePlatforms() {
    // ��ӵ�һ������Ծƽ̨��y=400, x=200��x=300
    Platform* jumpPlatform1 = new Platform(200, 400, 100, 30, this);
    jumpablePlatforms.append(jumpPlatform1);
}

qreal Battlefield::getFloorHeight() {
    // ����߶�������ƽ̨����
    return 600;
}

bool Battlefield::isCharacterOnGround(Character* character) const {
    if (!character || !groundPlatform) return false;
    
    // ��ȡ��ɫ����ײ��
    QRectF characterRect = character->getHitBox();
    
    // ����ɫ�Ƿ�վ��ƽ̨��
    return groundPlatform->isCharacterOnTop(characterRect);
}

bool Battlefield::isCharacterOnAnyPlatform(Character* character, qreal velocityY) const {
    if (!character) return false;
    
    // ��ȡ��ɫ����ײ��
    QRectF characterRect = character->getHitBox();
    
    // ֻ���½�ʱ���ƽ̨��ײ
    if (velocityY <= 0) return false;
    
    // ������ƽ̨
    if (groundPlatform && groundPlatform->isCharacterOnTop(characterRect)) {
        return true;
    }
    
    // ������п���Ծƽ̨
    for (Platform* platform : jumpablePlatforms) {
        if (platform && platform->isCharacterOnTop(characterRect)) {
            return true;
        }
    }
    
    return false;
}
