#include "Platform.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

Platform::Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : Item(parent, ""), 
      platformWidth(width), platformHeight(height) {
    
    // ����ƽ̨λ��
    setPos(x, y);
    
// ������ʯ����
    QPixmap rockPixmap(":/Items/Maps/Battlefield/rock.png");
    if (rockPixmap.isNull()) {
        qDebug() << "[ERROR] Failed to load rock texture!";
        return;
    }
    
    // ������Ҫ���ٸ���ʯͼƬ���������
    qreal rockWidth = rockPixmap.width();
    qreal rockHeight = rockPixmap.height();
    int tilesNeeded = static_cast<int>(width / rockWidth) + 1; // +1ȷ����ȫ����
    
    // ����ͳһ���ű�����ƥ��ƽ̨�߶�
    qreal scale = height / rockHeight;
    qreal scaledRockWidth = rockWidth * scale;
    
    // ���������ʯͼƬ����ƴ��
    for (int i = 0; i < tilesNeeded; ++i) {
        QGraphicsPixmapItem* rockTile = new QGraphicsPixmapItem(rockPixmap, this);
        rockTile->setPos(i * scaledRockWidth, 0); // ʹ�����ź�Ŀ��������
        rockTile->setScale(scale); // ͳһ���ű��ֱ���
        rockTile->setZValue(0);
    }
}

bool Platform::isCharacterOnTop(const QRectF& characterRect) const {
    QRectF platformRect = getPlatformRect();
    
    // ���ˮƽ�ص�
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // ����ɫ�ײ��Ƿ���ƽ̨��������
    qreal characterBottom = characterRect.bottom();
    qreal platformTop = platformRect.top();
    
    bool onTop = characterBottom >= platformTop - 2 && 
                characterBottom <= platformTop + 5;
                
    return horizontalOverlap && onTop;
}

bool Platform::isCharacterHittingFromBelow(const QRectF& characterRect, qreal velocityY) const {
    if (velocityY >= 0) return false; // ֻ��������˶�
    
    QRectF platformRect = getPlatformRect();
    
    // ���ˮƽ�ص�
    bool horizontalOverlap = characterRect.right() > platformRect.left() && 
                           characterRect.left() < platformRect.right();
    
    // ����ɫͷ���Ƿ�ײ��ƽ̨�ײ�
    bool hittingBottom = characterRect.top() <= platformRect.bottom() + 5 && 
                        characterRect.top() >= platformRect.bottom() - 10;
                        
    return horizontalOverlap && hittingBottom;
}

qreal Platform::getTopY() const {
    return pos().y();
}

qreal Platform::getBottomY() const {
    return pos().y() + platformHeight;
}

QRectF Platform::getPlatformRect() const {
    return QRectF(pos().x(), pos().y(), platformWidth, platformHeight);
}
