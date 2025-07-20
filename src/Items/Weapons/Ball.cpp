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
    setPos(-80, -140); // 调整位置
    setRotation(0);
    setZValue(2);
}

void Ball::advance(int phase) {
    if (phase == 0) return;
    // 如果已标记不活跃，停止运动
    if (!active) return;
    
    // 只有在未被挂载时才检查生存时间
    if (!isMounted()) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - creationTime > maxLifetime) {
            if (scene()) {
                this->deleteLater();
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
        qDebug() << "[DEBUG] Ball gravity applied: vy =" << velocity.y() << "gravity =" << currentGravity << "isThrown =" << isThrown;
    }
    gravityDebugCounter++;
    
    // 更新位置
    QPointF oldPos = scenePos();
    moveBy(velocity.x(), velocity.y());
    QPointF newPos = scenePos();
    
    // 调试输出
    static int debugCounter = 0;
    if (debugCounter % 30 == 0) {
        qDebug() << "[DEBUG] Ball pos:" << newPos << "velocity:" << velocity;
    }
    debugCounter++;
    
    // 检查是否落到地面或超出边界
    if (scene()) {
        BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
        if (battleScene) {
            // 获取地面高度
            qreal groundY = battleScene->getGroundHeight();
            if (newPos.y() >= groundY) {
                qDebug() << "[DEBUG] Ball hit ground, removing";
                // 只有当球是掉落武器时才从 BattleScene 的列表中移除
                // 投掷的球不在 fallingWeapons 列表中，不需要移除
                if (!shooter) {
                    QMetaObject::invokeMethod(battleScene, "removeFallingWeapon", Q_ARG(Weapon*, this));
                }
                this->deleteLater();
                return;
            }
        }
        
        // 检查是否超出屏幕边界
        qreal currentX = scenePos().x();
        qreal currentY = scenePos().y();
        if (currentX < -50 || currentX > 1330 || currentY > 1000) {
            qDebug() << "[DEBUG] Ball removed (off screen)";
            this->deleteLater();
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
            qDebug() << "[DEBUG] Ball hit character! Dealing 15 damage";
            int currentHP = character->getHP();
            character->setHP(std::max(0, currentHP - 15));
            
            // 重新绘制血条
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 移除铅球
            this->deleteLater();
            return;
        }
    }
    
    // 检查与Hero的碰撞
    Character* hero = battleScene->getHero();
    if (hero && hero != shooter) {
        if (hero->checkBulletCollision(ballPos)) {
            qDebug() << "[DEBUG] Ball hit hero! Dealing 15 damage";
            int currentHP = hero->getHP();
            hero->setHP(std::max(0, currentHP - 50));
            
            // 重新绘制血条
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 移除铅球
            this->deleteLater();
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
