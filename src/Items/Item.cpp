#include "Item.h"
#include "Maps/Battlefield.h"
#include <QDateTime>
#include <QDebug>

Item::Item(QGraphicsItem *parent, const QString &pixmapPath, bool mirrorPixmap) : QGraphicsItem(parent) {
    if (pixmapPath != "") {
        QPixmap pix(pixmapPath);
        if (mirrorPixmap) {
            pix = pix.transformed(QTransform().scale(-1, 1));
        }
        pixmapItem = new QGraphicsPixmapItem(pix, this);
    }
    lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
}

void Item::advance(int step) {
    if (step == 0) return; // ֻ��step=1ʱ��������״̬
    
    if (!gravityEnabled) return;
    
    // ����ʱ���
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    double deltaTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Ӧ������
    applyGravity(deltaTime);
}

void Item::applyGravity(double deltaTime) {
    if (!gravityEnabled) return;
    
    bool onGround = isOnGround();
    
    if (!onGround) {
        // Ӧ���������ٶ�
        velocity.setY(velocity.y() + gravity * deltaTime);
        
        // ����λ��
        QPointF newPos = pos() + velocity;
        setPos(newPos);
        
        // ���¼���Ƿ����
        if (isOnGround()) {
            // �����أ�ֹͣ��ֱ�˶�
            velocity.setY(0);
            
            // ����λ�õ�����
            if (scene()) {
                auto items = scene()->items();
                for (auto item : items) {
                    if (auto battlefield = dynamic_cast<Battlefield*>(item)) {
                        qreal groundHeight = battlefield->getFloorHeight();
                        QPointF adjustedPos = pos();
                        adjustedPos.setY(groundHeight - boundingRect().height());
                        setPos(adjustedPos);
                        break;
                    }
                }
            }
        }
    }
}

bool Item::isOnGround() const {
    if (!scene()) return false;
    
    // ����Ƿ��ڵ����ƽ̨��
    auto items = scene()->items();
    for (auto item : items) {
        if (auto battlefield = dynamic_cast<Battlefield*>(item)) {
            qreal groundHeight = battlefield->getFloorHeight();
            QRectF itemRect = sceneBoundingRect();
            
            // �����Ʒ�ײ��Ƿ�Ӵ�����ڵ���
            if (itemRect.bottom() >= groundHeight) {
                return true;
            }
            
            // �����Լ���Ƿ�����Ծƽ̨��
            // �����Ϊֻ�����棬�����Ҫ������չ
        }
    }
    
    return false;
}
