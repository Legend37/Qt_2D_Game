#include "Bullet.h"
#include "../Characters/Character.h"
#include "../../Scenes/BattleScene.h"
#include <QGraphicsScene>

Bullet::~Bullet() {}

void Bullet::advance(int phase) {
    if (phase == 0) return;
    
    // 每秒输出一次子弹坐标 (假设60fps，每60帧输出一次)
    static int debugCounter = 0;
    if (debugCounter % 60 == 0) { // 每60帧输出一次 (约每秒)
        qDebug() << "[DEBUG] Bullet scenePos:" << scenePos() << "center:" << getSceneCenter() << "velocity:" << velocity;
    }
    debugCounter++;
    
    moveBy(velocity.x(), velocity.y());
    
    // 检查碰撞
    checkCollisions();
    
    // Remove bullet if it goes off screen
    if (pos().x() < -1000 || pos().x() > 2000 || pos().y() < -1000 || pos().y() > 2000) {
        // qDebug() << "[DEBUG] Bullet removed (off screen)";
        scene()->removeItem(this);
        delete this;
        return;
    }
}

void Bullet::checkCollisions() {
    if (!scene()) return;
    
    // 尝试将场景转换为BattleScene
    BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
    if (!battleScene) {
        // qDebug() << "[DEBUG] Failed to cast scene to BattleScene";
        return;
    }
    
    // 获取当前子弹位置 - 使用子弹的中心点
    QPointF bulletPos = scenePos() + QPointF(10, 10); // 子弹中心点 (子弹是20x20，所以中心是+10,+10)
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
            // 发生碰撞，角色损失20点生命值
            int currentHP = character->getHP();
            character->setHP(std::max(0, currentHP - 20));
            // qDebug() << "[DEBUG] Bullet hit character! HP:" << currentHP << "->" << character->getHP();
            
            // 强制更新血量条
            battleScene->invalidate(battleScene->sceneRect(), QGraphicsScene::ForegroundLayer);
            
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
            // 发生碰撞，Hero损失20点生命值
            int currentHP = hero->getHP();
            hero->setHP(std::max(0, currentHP - 20));
            // qDebug() << "[DEBUG] Bullet hit hero! HP:" << currentHP << "->" << hero->getHP();
            
            // 强制更新血量条
            battleScene->invalidate(battleScene->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // 移除子弹
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
}

QPointF Bullet::getSceneCenter() const {
    // 子弹为20x20，中心点为+10,+10
    return scenePos() + QPointF(10, 10);
}
