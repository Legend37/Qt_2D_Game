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
    // 设置初始弹药数量为1（只能使用一次）
    setAmmo(1);
    
    // 记录创建时间
    creationTime = QDateTime::currentMSecsSinceEpoch();
    
    // 设置边界矩形
    setTransformOriginPoint(boundingRect().center());
    
    velocity = QPointF(0, 0);
    
    // 确保铅球初始状态是未挂载的，可以正常掉落
    unmount();
}

void Ball::mountToParent() {
    // 铅球的挂载逻辑，可以放大显示
    Weapon::mountToParent(); // 调用基类的挂载方法
    setScale(1.5); // 稍微放大一点
    setPos(33, -85); // 与其他武器位置一致
    setRotation(0);
    setZValue(2);
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
    qreal currentGravity = isThrown ? thrownGravity : gravity;
    velocity.setY(velocity.y() + currentGravity);
    
    // 调试输出重力应用情况
    static int gravityDebugCounter = 0;
    if (gravityDebugCounter % 15 == 0) {
        // qDebug() << "[DEBUG] Ball gravity applied: vy =" << velocity.y() << "gravity =" << currentGravity << "isThrown =" << isThrown;
    }
    gravityDebugCounter++;
    
    // 更新位置
    QPointF oldPos = scenePos();
    moveBy(velocity.x(), velocity.y());
    QPointF newPos = scenePos();
    
    // 调试输出
    static int debugCounter = 0;
    if (debugCounter % 30 == 0) {
        // qDebug() << "[DEBUG] Ball pos:" << newPos << "velocity:" << velocity;
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
                velocity = QPointF(0, 0);
                
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
    velocity = QPointF(vx, vy);
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
