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
    setupGrassElements(); // 重新启用草地显示
    setupIceBlock(); // 启用冰块显示
}

void Battlefield::setupGrassElements() {
    // 创建草地图像
    QPixmap grassPixmap(":/Items/Maps/Battlefield/grass.png");
    
    if (grassPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load grass.png";
        return;
    }
    
    qreal grassWidth = grassPixmap.width();
    qreal grassHeight = grassPixmap.height();
    qreal yPosition = 580; // 草地y位置，在地面平台上方
    
    // 第一个草地区域：覆盖(300-450)，宽度150像素
    qreal area1Start = 300;
    qreal area1Width = 150; // 450 - 300
    int tiles1Needed = static_cast<int>(std::ceil(area1Width / grassWidth));
    
    for (int i = 0; i < tiles1Needed; ++i) {
        QGraphicsPixmapItem* grassTile = new QGraphicsPixmapItem(grassPixmap, this);
        qreal tileX = area1Start + i * grassWidth - 40; // 向左偏移40像素
        
        // 如果最后一个瓦片超出区域边界，进行裁剪（基于原始位置计算）
        if ((area1Start + i * grassWidth) + grassWidth > area1Start + area1Width) {
            qreal remainingWidth = (area1Start + area1Width) - (area1Start + i * grassWidth);
            QPixmap clippedPixmap = grassPixmap.copy(0, 0, remainingWidth, grassPixmap.height());
            grassTile->setPixmap(clippedPixmap);
        }
        
        grassTile->setPos(tileX, yPosition); // 使用偏移后的位置
        grassTile->setZValue(1); // 确保草地在背景之上
    }
    
    // 第二个草地区域：覆盖(900-1050)，宽度150像素
    qreal area2Start = 900;
    qreal area2Width = 150; // 1050 - 900
    int tiles2Needed = static_cast<int>(std::ceil(area2Width / grassWidth));
    
    for (int i = 0; i < tiles2Needed; ++i) {
        QGraphicsPixmapItem* grassTile = new QGraphicsPixmapItem(grassPixmap, this);
        qreal tileX = area2Start + i * grassWidth - 40; // 向左偏移40像素
        
        // 如果最后一个瓦片超出区域边界，进行裁剪（基于原始位置计算）
        if ((area2Start + i * grassWidth) + grassWidth > area2Start + area2Width) {
            qreal remainingWidth = (area2Start + area2Width) - (area2Start + i * grassWidth);
            QPixmap clippedPixmap = grassPixmap.copy(0, 0, remainingWidth, grassPixmap.height());
            grassTile->setPixmap(clippedPixmap);
        }
        
        grassTile->setPos(tileX, yPosition); // 使用偏移后的位置
        grassTile->setZValue(1); // 确保草地在背景之上
    }
    
    qDebug() << "[DEBUG] Grass elements created - Area1: tiles=" << tiles1Needed 
             << "Area2: tiles=" << tiles2Needed << "at y=" << yPosition;
}

void Battlefield::setupIceBlock() {
    // 创建冰块贴图，位置在450到750之间，厚度与地面平台一致
    QPixmap icePixmap(":/Items/Maps/Battlefield/Ice_block.png");
    if (icePixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load Ice_block.png";
        return;
    }
    
    // 冰块位置：x从450到750（宽度300像素），厚度与地面平台一致（40像素）
    qreal iceX = 450;
    qreal iceY = 600; // 与地面平台重合
    qreal iceWidth = 300; // 750 - 450
    qreal iceThickness = 40; // 与地面平台相同的厚度
    
    // 计算需要的冰块瓦片数量
    qreal originalWidth = icePixmap.width();
    qreal originalHeight = icePixmap.height();
    int tilesNeeded = static_cast<int>(std::ceil(iceWidth / originalWidth));
    
    for (int i = 0; i < tilesNeeded; ++i) {
        QGraphicsPixmapItem* iceTile = new QGraphicsPixmapItem(this);
        qreal tileX = iceX + i * originalWidth;
        
        QPixmap processedPixmap;
        
        // 首先处理水平裁剪（如果最后一个瓦片超出区域边界）
        if (tileX + originalWidth > iceX + iceWidth) {
            qreal remainingWidth = (iceX + iceWidth) - tileX;
            processedPixmap = icePixmap.copy(0, 0, remainingWidth, originalHeight);
        } else {
            processedPixmap = icePixmap;
        }
        
        // 然后处理垂直裁剪（限制厚度为40像素）
        if (processedPixmap.height() > iceThickness) {
            processedPixmap = processedPixmap.copy(0, 0, processedPixmap.width(), iceThickness);
        }
        
        iceTile->setPixmap(processedPixmap);
        iceTile->setPos(tileX, iceY);
        iceTile->setZValue(2); // 确保冰块在草地之上
    }
    
    qDebug() << "[DEBUG] Ice block created from x=" << iceX << "to x=" << (iceX + iceWidth) 
             << "at y=" << iceY << "with thickness=" << iceThickness << "and" << tilesNeeded << "tiles";
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
    // 添加第一个可跳跃平台：y=450, x=200到x=400
    Platform* jumpPlatform1 = new Platform(200, 450, 200, 30, this);
    jumpablePlatforms.append(jumpPlatform1);
    
    // 添加第二个可跳跃平台：y=400, x=800到x=1000 (与第一个平台相同宽度)
    Platform* jumpPlatform2 = new Platform(800, 400, 200, 30, this);
    jumpablePlatforms.append(jumpPlatform2);
    
    // 添加第三个可跳跃平台：y=300, x=500到x=700
    Platform* jumpPlatform3 = new Platform(500, 300, 200, 30, this);
    jumpablePlatforms.append(jumpPlatform3);
    
    // 添加第四个可跳跃平台：y=230, x=72处，宽度150像素
    Platform* jumpPlatform4 = new Platform(72, 230, 150, 30, this);
    jumpablePlatforms.append(jumpPlatform4);
    
    // 添加第五个可跳跃平台：y=200, x=930处，宽度150像素
    Platform* jumpPlatform5 = new Platform(930, 200, 150, 30, this);
    jumpablePlatforms.append(jumpPlatform5);
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
    
    // 如果velocityY为0，表示用于跳跃检测，检查所有平台
    // 如果velocityY > 0，表示下降时的碰撞检测
    if (velocityY > 0) {
        // 只在下降时检查平台碰撞
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
    } else {
        // velocityY <= 0，用于跳跃检测，检查所有平台
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
    }
    
    return false;
}
