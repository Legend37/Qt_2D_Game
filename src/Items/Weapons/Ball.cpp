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
    // Set initial ammo to 1 (can only be used once)
    setAmmo(1);
    
    // Record creation time
    creationTime = QDateTime::currentMSecsSinceEpoch();
    
    // Set bounding rectangle
    setTransformOriginPoint(boundingRect().center());
    
    ballVelocity = QPointF(0, 0);
    
    // Ensure ball is initially unmounted and can drop normally
    unmount();
}

void Ball::mountToParent() {
    // Ball mount logic, can be enlarged for display
    Weapon::mountToParent(); // Call base class mount method
    setScale(1.5); // Slightly enlarge
    setPos(33, -85); // Same position as other weapons
    setRotation(0);
    setZValue(2);
    
    // Mark Ball as picked up
    hasBeenPickedUp = true;
}

void Ball::unmount() {
    // Call base class unmount method
    Mountable::unmount();
    
    // Clear shooter, treat as natural drop
    shooter = nullptr;
    
    // Set different survival time depending on whether it was picked up
    if (hasBeenPickedUp) {
        // Ball sets 0.5s survival time after being dropped
        groundTimer = QDateTime::currentMSecsSinceEpoch() - 9500; // Set to 9.5s ago, remaining 0.5s
    } else {
        // Ball dropped naturally, reset groundTimer for full 10s survival
        groundTimer = 0;
    }
    
    // Reset Ball state
    ballVelocity = QPointF(0, 0); // Reset velocity to 0
    isThrown = false; // Set to non-thrown mode
    active = true; // Ensure Ball is active for deletion logic
}

void Ball::advance(int phase) {
    if (phase == 0) return;
    // 如果已标记不活跃，停止运动
    if (!active) {
        // qDebug() << "[DEBUG] Ball" << this << "is inactive, skipping advance()";
        return;
    }
    
    // 只有在未被挂载时才检查生存时间
    if (!isMounted()) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - creationTime > maxLifetime) {
            qDebug() << "[DEBUG] Ball lifetime expired - Ball pointer:" << this;
            qDebug() << "[DEBUG] Ball lifetime expired - Active status:" << active;
            qDebug() << "[DEBUG] Ball lifetime expired - Time alive:" << (currentTime - creationTime) << "ms";
            
            // 防止重复删除
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive when lifetime expired, skipping";
                return;
            }
            active = false;
            
            if (scene()) {
                scene()->removeItem(this);
                qDebug() << "[DEBUG] Ball removed from scene (lifetime expired)";
                
                // 使用定时器延迟删除，避免立即删除导致的问题
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
    
    // 应用重力 - 根据是否为投掷模式使用不同的重力值
    // 注意：掉落模式使用与Character一致的重力值0.008
    qreal currentGravity = isThrown ? thrownGravity : 0.008; // 使用与Character一致的重力
    ballVelocity.setY(ballVelocity.y() + currentGravity);
    
    // 调试输出重力应用情况
    static int gravityDebugCounter = 0;
    if (gravityDebugCounter % 15 == 0) {
        // qDebug() << "[DEBUG] Ball gravity applied: vy =" << ballVelocity.y() << "gravity =" << currentGravity << "isThrown =" << isThrown;
    }
    gravityDebugCounter++;
    
    // 更新位置
    QPointF oldPos = scenePos();
    moveBy(ballVelocity.x(), ballVelocity.y());
    QPointF newPos = scenePos();
    
    // 调试输出
    static int debugCounter = 0;
    if (debugCounter % 30 == 0) {
        // qDebug() << "[DEBUG] Ball pos:" << newPos << "velocity:" << ballVelocity;
    }
    debugCounter++;
    
    // 检查是否落到地面或超出边界
    if (scene()) {
        BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
        if (battleScene) {
            // 获取地面高度
            qreal groundY = battleScene->getGroundHeight();
            if (newPos.y() >= groundY) {
                // 球落地，停止运动
                setPos(scenePos().x(), groundY);
                ballVelocity = QPointF(0, 0);
                
                // 检查是否是投掷的Ball（有shooter）
                if (shooter) {
                    
                    // 防止重复删除
                    if (!active) {
                        return;
                    }
                    active = false;
                    
                    // 从场景中移除
                    if (scene()) {
                        scene()->removeItem(this);
                    }
                    
                    // 立即删除
                    QTimer::singleShot(0, [this]() {
                        try {
                            delete this;
                        } catch (...) {
                            qDebug() << "[DEBUG] ERROR during thrown Ball deletion";
                        }
                    });
                    return;
                } else {
                    // 掉落的Ball（无shooter）进入10秒计时逻辑
                    // 如果尚未开始地面计时，开始计时
                    if (groundTimer == 0) {
                        groundTimer = QDateTime::currentMSecsSinceEpoch();
                    }
                    
                    // 检查是否已在地面停留10秒
                    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
                    if (currentTime - groundTimer >= 10000) { // 10秒
                        
                        // 标记为非活跃状态，防止重复删除
                        if (!active) {
                            qDebug() << "[DEBUG] Dropped Ball already inactive, skipping deletion";
                            return;
                        }
                        active = false;
                        
                        try {
                            battleScene->removeFallingWeapon(this);
                        } catch (...) {
                        }
                        
                        // 从场景中移除
                        try {
                            battleScene->removeItem(this);
                        } catch (...) {
                            qDebug() << "[DEBUG] ERROR: Exception while removing from scene";
                        }
                        
                        // 使用定时器延迟删除，避免立即删除导致的问题
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
        
        // 检查是否超出屏幕边界
        qreal currentX = scenePos().x();
        qreal currentY = scenePos().y();
        if (currentX < -50 || currentX > 1330 || currentY > 1000) {
            
            // 防止重复删除
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive when off-screen, skipping";
                return;
            }
            active = false;
            
            // 从场景中移除
            if (scene()) {
                scene()->removeItem(this);
                qDebug() << "[DEBUG] Ball removed from scene (off-screen)";
            }
            
            // 使用定时器延迟删除，避免立即删除导致的问题
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
    
    // 检查碰撞
    checkCollisions();
}

void Ball::checkCollisions() {
    // 只有被投掷后且活跃状态才检测碰撞
    if (!scene() || !shooter || !active) return;
    BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
    if (!battleScene) return;
    
    QPointF ballPos = scenePos();
    
    // 检查与角色的碰撞
    Character* character = battleScene->getCharacter();
    if (character && character != shooter) {
        if (character->checkBulletCollision(ballPos)) {
            
            // 防止重复删除
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive during collision, skipping";
                return;
            }
            active = false;
            
            character->takeDamage(50, DamageType::Bullet); // 球被视为投射物伤害（类似子弹）
            
            // 重新绘制血条
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 从场景中移除
            if (scene()) {
                scene()->removeItem(this);
            }
            
            // 使用定时器延迟删除，避免立即删除导致的问题
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
    
    // 检查与Hero的碰撞
    Character* hero = battleScene->getHero();
    if (hero && hero != shooter) {
        if (hero->checkBulletCollision(ballPos)) {
            
            // 防止重复删除
            if (!active) {
                qDebug() << "[DEBUG] Ball already inactive during hero collision, skipping";
                return;
            }
            active = false;
            
            hero->takeDamage(50, DamageType::Bullet); // 球被视为投射物伤害（类似子弹）
            
            // 重新绘制血条
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 从场景中移除
            if (scene()) {
                scene()->removeItem(this);
            }
            
            // 使用定时器延迟删除，避免立即删除导致的问题
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
    ballVelocity = QPointF(vx, vy);
    // 只有当有非零速度时才设置为投掷模式
    if (vx != 0 || vy != 0) {
        isThrown = true;
    }
}

void Ball::setThrownMode(bool thrown) {
    isThrown = thrown;
}

bool Ball::isMounted() const {
    return Mountable::isMounted();
}

void Ball::deleteLater() {
    // 标记不活跃并立即从场景移除，停止后续 advance 调用
    if (!active) return;
    active = false;
    if (scene()) {
        scene()->removeItem(this);
    }
    // 延迟删除，确保当前调用栈安全
    QTimer::singleShot(0, [this]() {
        delete this;
    });
}
