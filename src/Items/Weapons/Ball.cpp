#include "Ball.h"
#include "../Characters/Character.h"
#include "../../Scenes/BattleScene.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

Ball::Ball(QGraphicsItem *parent) 
    : Item(parent, ":/Items/Weapon/ball.png", false),
      Weapon(parent, ":/Items/Weapon/ball.png", "Ball") {
    // ���ó�ʼ��ҩ����Ϊ1��ֻ��ʹ��һ�Σ�
    setAmmo(1);
    
    // ��¼����ʱ��
    creationTime = QDateTime::currentMSecsSinceEpoch();
    
    // ���ñ߽����
    setTransformOriginPoint(boundingRect().center());
    
    velocity = QPointF(0, 0);
    
    // ȷ��Ǧ���ʼ״̬��δ���صģ�������������
    unmount();
}

void Ball::mountToParent() {
    // Ǧ��Ĺ����߼������ԷŴ���ʾ
    Weapon::mountToParent(); // ���û���Ĺ��ط���
    setScale(1.5); // ��΢�Ŵ�һ��
    setPos(33, -85); // ����������λ��һ��
    setRotation(0);
    setZValue(2);
}

void Ball::advance(int phase) {
    if (phase == 0) return;
    // ����ѱ�ǲ���Ծ��ֹͣ�˶�
    if (!active) {
        // qDebug() << "[DEBUG] Ball" << this << "is inactive, skipping advance()";
        return;
    }
    
    // ֻ����δ������ʱ�ż������ʱ��
    if (!isMounted()) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - creationTime > maxLifetime) {
            qDebug() << "[DEBUG] Ball lifetime expired - Ball pointer:" << this;
            qDebug() << "[DEBUG] Ball lifetime expired - Active status:" << active;
            qDebug() << "[DEBUG] Ball lifetime expired - Time alive:" << (currentTime - creationTime) << "ms";
            
            // ��ֹ�ظ�ɾ��
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive when lifetime expired, skipping";
                return;
            }
            active = false;
            
            if (scene()) {
                scene()->removeItem(this);
                qDebug() << "[DEBUG] Ball removed from scene (lifetime expired)";
                
                // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
                qDebug() << "[DEBUG] Scheduling Ball deletion (lifetime expired)";
                QTimer::singleShot(0, [this]() {
                    qDebug() << "[DEBUG] Lambda: Deleting Ball (lifetime expired)" << this;
                    try {
                        delete this;
                        qDebug() << "[DEBUG] Lambda: Ball lifetime deletion completed";
                    } catch (...) {
                        qDebug() << "[DEBUG] Lambda: ERROR during Ball lifetime deletion";
                    }
                });
            }
            return;
        }
    }
    
    // Ӧ������ - �����Ƿ�ΪͶ��ģʽʹ�ò�ͬ������ֵ
    qreal currentGravity = isThrown ? thrownGravity : gravity;
    velocity.setY(velocity.y() + currentGravity);
    
    // �����������Ӧ�����
    static int gravityDebugCounter = 0;
    if (gravityDebugCounter % 15 == 0) {
        // qDebug() << "[DEBUG] Ball gravity applied: vy =" << velocity.y() << "gravity =" << currentGravity << "isThrown =" << isThrown;
    }
    gravityDebugCounter++;
    
    // ����λ��
    QPointF oldPos = scenePos();
    moveBy(velocity.x(), velocity.y());
    QPointF newPos = scenePos();
    
    // �������
    static int debugCounter = 0;
    if (debugCounter % 30 == 0) {
        // qDebug() << "[DEBUG] Ball pos:" << newPos << "velocity:" << velocity;
    }
    debugCounter++;
    
    // ����Ƿ��䵽����򳬳��߽�
    if (scene()) {
        BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
        if (battleScene) {
            // ��ȡ����߶�
            qreal groundY = battleScene->getGroundHeight();
            if (newPos.y() >= groundY) {
                // ����أ�ֹͣ�˶�
                setPos(scenePos().x(), groundY);
                velocity = QPointF(0, 0);
                
                // ����Ƿ���Ͷ����Ball����shooter��
                if (shooter) {
                    
                    // ��ֹ�ظ�ɾ��
                    if (!active) {
                        return;
                    }
                    active = false;
                    
                    // �ӳ������Ƴ�
                    if (scene()) {
                        scene()->removeItem(this);
                    }
                    
                    // ����ɾ��
                    QTimer::singleShot(0, [this]() {
                        try {
                            delete this;
                        } catch (...) {
                            qDebug() << "[DEBUG] ERROR during thrown Ball deletion";
                        }
                    });
                    return;
                } else {
                    // �����Ball����shooter������10���ʱ�߼�
                    // �����δ��ʼ�����ʱ����ʼ��ʱ
                    if (groundTimer == 0) {
                        groundTimer = QDateTime::currentMSecsSinceEpoch();
                    }
                    
                    // ����Ƿ����ڵ���ͣ��10��
                    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
                    if (currentTime - groundTimer >= 10000) { // 10��
                        
                        // ���Ϊ�ǻ�Ծ״̬����ֹ�ظ�ɾ��
                        if (!active) {
                            qDebug() << "[DEBUG] Dropped Ball already inactive, skipping deletion";
                            return;
                        }
                        active = false;
                        
                        try {
                            battleScene->removeFallingWeapon(this);
                        } catch (...) {
                        }
                        
                        // �ӳ������Ƴ�
                        try {
                            battleScene->removeItem(this);
                        } catch (...) {
                            qDebug() << "[DEBUG] ERROR: Exception while removing from scene";
                        }
                        
                        // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
                        QTimer::singleShot(0, [this]() {
                            try {
                                delete this;
                            } catch (...) {
                                qDebug() << "[DEBUG] Lambda: ERROR during dropped Ball deletion";
                            }
                        });
                        qDebug() << "[DEBUG] Dropped Ball deletion scheduled, returning from advance()";
                        return;
                    }
                }
            }
        }
        
        // ����Ƿ񳬳���Ļ�߽�
        qreal currentX = scenePos().x();
        qreal currentY = scenePos().y();
        if (currentX < -50 || currentX > 1330 || currentY > 1000) {
            
            // ��ֹ�ظ�ɾ��
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive when off-screen, skipping";
                return;
            }
            active = false;
            
            // �ӳ������Ƴ�
            if (scene()) {
                scene()->removeItem(this);
                qDebug() << "[DEBUG] Ball removed from scene (off-screen)";
            }
            
            // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
            QTimer::singleShot(0, [this]() {
                try {
                    delete this;
                } catch (...) {
                    qDebug() << "[DEBUG] Lambda: ERROR during Ball off-screen deletion";
                }
            });
            return;
        }
    }
    
    // �����ײ
    checkCollisions();
}

void Ball::checkCollisions() {
    // ֻ�б�Ͷ�����һ�Ծ״̬�ż����ײ
    if (!scene() || !shooter || !active) return;
    BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
    if (!battleScene) return;
    
    QPointF ballPos = scenePos();
    
    // ������ɫ����ײ
    Character* character = battleScene->getCharacter();
    if (character && character != shooter) {
        if (character->checkBulletCollision(ballPos)) {
            
            // ��ֹ�ظ�ɾ��
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive during collision, skipping";
                return;
            }
            active = false;
            
            character->takeDamage(50, DamageType::Bullet); // ����ΪͶ�����˺��������ӵ���
            
            // ���»���Ѫ��
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �ӳ������Ƴ�
            if (scene()) {
                scene()->removeItem(this);
            }
            
            // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
            QTimer::singleShot(0, [this]() {
                try {
                    delete this;
                } catch (...) {
                    qDebug() << "[DEBUG] Lambda: ERROR during Ball collision deletion";
                }
            });
            return;
        }
    }
    
    // �����Hero����ײ
    Character* hero = battleScene->getHero();
    if (hero && hero != shooter) {
        if (hero->checkBulletCollision(ballPos)) {
            
            // ��ֹ�ظ�ɾ��
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive during hero collision, skipping";
                return;
            }
            active = false;
            
            hero->takeDamage(50, DamageType::Bullet); // ����ΪͶ�����˺��������ӵ���
            
            // ���»���Ѫ��
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �ӳ������Ƴ�
            if (scene()) {
                scene()->removeItem(this);
            }
            
            // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
            QTimer::singleShot(0, [this]() {
                try {
                    delete this;
                } catch (...) {
                    qDebug() << "[DEBUG] Lambda: ERROR during Ball hero collision deletion";
                }
            });
            return;
        }
    }
}

QPointF Ball::getSceneCenter() const {
    return scenePos();
}

void Ball::setVelocity(qreal vx, qreal vy) {
    velocity = QPointF(vx, vy);
}

void Ball::setThrownMode(bool thrown) {
    isThrown = thrown;
}

bool Ball::isMounted() const {
    return Mountable::isMounted();
}

void Ball::deleteLater() {
    // ��ǲ���Ծ�������ӳ����Ƴ���ֹͣ���� advance ����
    if (!active) return;
    active = false;
    if (scene()) {
        scene()->removeItem(this);
    }
    // �ӳ�ɾ����ȷ����ǰ����ջ��ȫ
    QTimer::singleShot(0, [this]() {
        delete this;
    });
}
