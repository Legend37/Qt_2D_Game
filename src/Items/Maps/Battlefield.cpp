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
    setupGrassElements(); // �������òݵ���ʾ
}

void Battlefield::setupGrassElements() {
    // �����ݵ�ͼ��
    QPixmap grassPixmap(":/Items/Maps/Battlefield/grass.png");
    
    if (grassPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load grass.png";
        return;
    }
    
    qreal grassWidth = grassPixmap.width();
    qreal grassHeight = grassPixmap.height();
    qreal yPosition = 580; // �ݵ�yλ�ã��ڵ���ƽ̨�Ϸ�
    
    // ��һ���ݵ����򣺸���(300-450)�����150����
    qreal area1Start = 300;
    qreal area1Width = 150; // 450 - 300
    int tiles1Needed = static_cast<int>(std::ceil(area1Width / grassWidth));
    
    for (int i = 0; i < tiles1Needed; ++i) {
        QGraphicsPixmapItem* grassTile = new QGraphicsPixmapItem(grassPixmap, this);
        qreal tileX = area1Start + i * grassWidth - 40; // ����ƫ��40����
        
        // ������һ����Ƭ��������߽磬���вü�������ԭʼλ�ü��㣩
        if ((area1Start + i * grassWidth) + grassWidth > area1Start + area1Width) {
            qreal remainingWidth = (area1Start + area1Width) - (area1Start + i * grassWidth);
            QPixmap clippedPixmap = grassPixmap.copy(0, 0, remainingWidth, grassPixmap.height());
            grassTile->setPixmap(clippedPixmap);
        }
        
        grassTile->setPos(tileX, yPosition); // ʹ��ƫ�ƺ��λ��
        grassTile->setZValue(1); // ȷ���ݵ��ڱ���֮��
    }
    
    // �ڶ����ݵ����򣺸���(900-1050)�����150����
    qreal area2Start = 900;
    qreal area2Width = 150; // 1050 - 900
    int tiles2Needed = static_cast<int>(std::ceil(area2Width / grassWidth));
    
    for (int i = 0; i < tiles2Needed; ++i) {
        QGraphicsPixmapItem* grassTile = new QGraphicsPixmapItem(grassPixmap, this);
        qreal tileX = area2Start + i * grassWidth - 40; // ����ƫ��40����
        
        // ������һ����Ƭ��������߽磬���вü�������ԭʼλ�ü��㣩
        if ((area2Start + i * grassWidth) + grassWidth > area2Start + area2Width) {
            qreal remainingWidth = (area2Start + area2Width) - (area2Start + i * grassWidth);
            QPixmap clippedPixmap = grassPixmap.copy(0, 0, remainingWidth, grassPixmap.height());
            grassTile->setPixmap(clippedPixmap);
        }
        
        grassTile->setPos(tileX, yPosition); // ʹ��ƫ�ƺ��λ��
        grassTile->setZValue(1); // ȷ���ݵ��ڱ���֮��
    }
    
    qDebug() << "[DEBUG] Grass elements created - Area1: tiles=" << tiles1Needed 
             << "Area2: tiles=" << tiles2Needed << "at y=" << yPosition;
}

void Battlefield::setupIceBlock() {
    // ������ͼ��ʾ�ѱ�ɾ��
    /*
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
    */
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
    // ��ӵ�һ������Ծƽ̨��y=450, x=200��x=400
    Platform* jumpPlatform1 = new Platform(200, 450, 200, 30, this);
    jumpablePlatforms.append(jumpPlatform1);
    
    // ��ӵڶ�������Ծƽ̨��y=400, x=800��x=1000 (���һ��ƽ̨��ͬ���)
    Platform* jumpPlatform2 = new Platform(800, 400, 200, 30, this);
    jumpablePlatforms.append(jumpPlatform2);
    
    // ��ӵ���������Ծƽ̨��y=300, x=500��x=700
    Platform* jumpPlatform3 = new Platform(500, 300, 200, 30, this);
    jumpablePlatforms.append(jumpPlatform3);
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
    
    // ���velocityYΪ0����ʾ������Ծ��⣬�������ƽ̨
    // ���velocityY > 0����ʾ�½�ʱ����ײ���
    if (velocityY > 0) {
        // ֻ���½�ʱ���ƽ̨��ײ
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
    } else {
        // velocityY <= 0��������Ծ��⣬�������ƽ̨
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
    }
    
    return false;
}
