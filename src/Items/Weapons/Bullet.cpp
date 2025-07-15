#include "Bullet.h"
#include "../Characters/Character.h"
#include "../../Scenes/BattleScene.h"
#include <QGraphicsScene>

Bullet::~Bullet() {}

void Bullet::advance(int phase) {
    if (phase == 0) return;
    
    // ÿ�����һ���ӵ����� (����60fps��ÿ60֡���һ��)
    static int debugCounter = 0;
    if (debugCounter % 60 == 0) { // ÿ60֡���һ�� (Լÿ��)
        qDebug() << "[DEBUG] Bullet scenePos:" << scenePos() << "center:" << getSceneCenter() << "velocity:" << velocity;
    }
    debugCounter++;
    
    moveBy(velocity.x(), velocity.y());
    
    // �����ײ
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
    
    // ���Խ�����ת��ΪBattleScene
    BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
    if (!battleScene) {
        // qDebug() << "[DEBUG] Failed to cast scene to BattleScene";
        return;
    }
    
    // ��ȡ��ǰ�ӵ�λ�� - ʹ���ӵ������ĵ�
    QPointF bulletPos = scenePos() + QPointF(10, 10); // �ӵ����ĵ� (�ӵ���20x20������������+10,+10)
    // qDebug() << "[DEBUG] Checking bullet collision at center pos:" << bulletPos;
    
    // ������ɫ����ײ
    Character* character = battleScene->getCharacter();
    if (character && character != shooter) {
        QRectF hitBox = character->getHitBox();
        // qDebug() << "[DEBUG] Character hitbox:" << hitBox << "Character pos:" << character->pos();
        // qDebug() << "[DEBUG] Bullet pos:" << bulletPos << "Bullet scenePos:" << scenePos() << "Bullet pos():" << pos();
        bool collision = character->checkBulletCollision(bulletPos);
        // qDebug() << "[DEBUG] Character collision result:" << collision;
        if (collision) {
            // ������ײ����ɫ��ʧ20������ֵ
            int currentHP = character->getHP();
            character->setHP(std::max(0, currentHP - 20));
            // qDebug() << "[DEBUG] Bullet hit character! HP:" << currentHP << "->" << character->getHP();
            
            // ǿ�Ƹ���Ѫ����
            battleScene->invalidate(battleScene->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �Ƴ��ӵ�
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
    
    // �����Hero����ײ
    Character* hero = battleScene->getHero();
    if (hero && hero != shooter) {
        QRectF hitBox = hero->getHitBox();
        // qDebug() << "[DEBUG] Hero hitbox:" << hitBox << "Hero pos:" << hero->pos();
        // qDebug() << "[DEBUG] Bullet pos:" << bulletPos << "Bullet scenePos:" << scenePos() << "Bullet pos():" << pos();
        bool collision = hero->checkBulletCollision(bulletPos);
        // qDebug() << "[DEBUG] Hero collision result:" << collision;
        if (collision) {
            // ������ײ��Hero��ʧ20������ֵ
            int currentHP = hero->getHP();
            hero->setHP(std::max(0, currentHP - 20));
            // qDebug() << "[DEBUG] Bullet hit hero! HP:" << currentHP << "->" << hero->getHP();
            
            // ǿ�Ƹ���Ѫ����
            battleScene->invalidate(battleScene->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �Ƴ��ӵ�
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
}

QPointF Bullet::getSceneCenter() const {
    // �ӵ�Ϊ20x20�����ĵ�Ϊ+10,+10
    return scenePos() + QPointF(10, 10);
}
