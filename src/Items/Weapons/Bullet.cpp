#include "Bullet.h"
#include "../Characters/Character.h"
#include "../../Scenes/BattleScene.h"
#include <QGraphicsScene>
#include <QDebug>

Bullet::~Bullet() {}

void Bullet::advance(int phase) {
    if (phase == 0) return;
    
    // 每秒输出一次子弹坐标 (假设60fps，每60帧输出一次)
    static int debugCounter = 0;
    if (debugCounter % 60 == 0) { // 每60帧输出一次 (约每秒)
        // qDebug() << "[DEBUG] Bullet scenePos:" << scenePos() << "center:" << getSceneCenter() << "velocity:" << velocity;
    }
    debugCounter++;
    
    QPointF oldPos = scenePos();
    moveBy(velocity.x(), velocity.y());
    QPointF newPos = scenePos();
    
    // 每帧都输出移动信息（临时调试）
    if (debugCounter % 10 == 0) {
        // qDebug() << "[DEBUG] Bullet moved from:" << oldPos << "to:" << newPos << "velocity:" << velocity;
    }
    
    // 检查碰撞
    checkCollisions();
    
    // Remove bullet if it goes off screen
    qreal currentX = scenePos().x();
    qreal currentY = scenePos().y();
    if (currentX < -50 || currentX > 1330 || currentY < -1000 || currentY > 2000) {
        // qDebug() << "[DEBUG] Bullet removed (off screen) - pos:" << scenePos() << "bounds: x[-50,1330] y[-1000,2000]";
        scene()->removeItem(this);
        delete this;
        return;
    }
}

void Bullet::checkCollisions() {
    if (!scene()) {
        // qDebug() << "[DEBUG] Bullet checkCollisions: no scene";
        return;
    }
    
    // 尝试将场景转换为BattleScene
    BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
    if (!battleScene) {
        // qDebug() << "[DEBUG] Failed to cast scene to BattleScene";
        return;
    }
    
    // qDebug() << "[DEBUG] Bullet checkCollisions: pos=" << scenePos();
    
    // 获取当前子弹位置 - 使用子弹的中心点
    QPointF bulletPos = scenePos(); // 子弹的中心点就是它的位置
    // qDebug() << "[DEBUG] Checking bullet collision at center pos:" << bulletPos;
    
    // 检查与角色的碰撞
    Character* character = battleScene->getCharacter();
    if (character && character != shooter) {
        QRectF hitBox = character->getHitBox();
        // qDebug() << "[DEBUG] Character hitbox:" << hitBox << "Character pos:" << character->pos();
        // qDebug() << "[DEBUG] Bullet pos:" << bulletPos << "Bullet scenePos:" << scenePos() << "Bullet pos():" << pos();
        bool collision = character->checkBulletCollision(bulletPos);
        // qDebug() << "[DEBUG] Character collision result:" << collision;
        if (collision) {
            // 发生碰撞，角色损失动态伤害值
            int currentHP = character->getHP();
            character->setHP(std::max(0, currentHP - damage));
            // qDebug() << "[DEBUG] Bullet hit character! Damage:" << damage << "HP:" << currentHP << "->" << character->getHP();
            
            // 重新绘制血条
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 移除子弹
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
    
    // 检查与Hero的碰撞
    Character* hero = battleScene->getHero();
    if (hero && hero != shooter) {
        QRectF hitBox = hero->getHitBox();
        // qDebug() << "[DEBUG] Hero hitbox:" << hitBox << "Hero pos:" << hero->pos();
        // qDebug() << "[DEBUG] Bullet pos:" << bulletPos << "Bullet scenePos:" << scenePos() << "Bullet pos():" << pos();
        bool collision = hero->checkBulletCollision(bulletPos);
        // qDebug() << "[DEBUG] Hero collision result:" << collision;
        if (collision) {
            // 发生碰撞，Hero损失动态伤害值
            int currentHP = hero->getHP();
            hero->setHP(std::max(0, currentHP - damage));
            // qDebug() << "[DEBUG] Bullet hit hero! Damage:" << damage << "HP:" << currentHP << "->" << hero->getHP();
            
            // 重新绘制血条
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 移除子弹
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
}

QPointF Bullet::getSceneCenter() const {
    // 子弹矩形是(-10, -10, 20, 20)，所以中心点需要加上偏移量
    // 矩形的中心点相对于item原点的偏移是(0, 0)，因为矩形是以(-10, -10)为左上角
    return scenePos();
}
