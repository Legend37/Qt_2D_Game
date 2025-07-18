#include "Bullet.h"
#include "../Characters/Character.h"
#include "../../Scenes/BattleScene.h"
#include <QGraphicsScene>
#include <QDebug>

Bullet::~Bullet() {}

void Bullet::advance(int phase) {
    if (phase == 0) return;
    
    // ÿ�����һ���ӵ����� (����60fps��ÿ60֡���һ��)
    static int debugCounter = 0;
    if (debugCounter % 60 == 0) { // ÿ60֡���һ�� (Լÿ��)
        // qDebug() << "[DEBUG] Bullet scenePos:" << scenePos() << "center:" << getSceneCenter() << "velocity:" << velocity;
    }
    debugCounter++;
    
    QPointF oldPos = scenePos();
    moveBy(velocity.x(), velocity.y());
    QPointF newPos = scenePos();
    
    // ÿ֡������ƶ���Ϣ����ʱ���ԣ�
    if (debugCounter % 10 == 0) {
        // qDebug() << "[DEBUG] Bullet moved from:" << oldPos << "to:" << newPos << "velocity:" << velocity;
    }
    
    // �����ײ
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
    
    // ���Խ�����ת��ΪBattleScene
    BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
    if (!battleScene) {
        // qDebug() << "[DEBUG] Failed to cast scene to BattleScene";
        return;
    }
    
    // qDebug() << "[DEBUG] Bullet checkCollisions: pos=" << scenePos();
    
    // ��ȡ��ǰ�ӵ�λ�� - ʹ���ӵ������ĵ�
    QPointF bulletPos = scenePos(); // �ӵ������ĵ��������λ��
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
            // ������ײ����ɫ��ʧ��̬�˺�ֵ
            int currentHP = character->getHP();
            character->setHP(std::max(0, currentHP - damage));
            // qDebug() << "[DEBUG] Bullet hit character! Damage:" << damage << "HP:" << currentHP << "->" << character->getHP();
            
            // ���»���Ѫ��
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
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
            // ������ײ��Hero��ʧ��̬�˺�ֵ
            int currentHP = hero->getHP();
            hero->setHP(std::max(0, currentHP - damage));
            // qDebug() << "[DEBUG] Bullet hit hero! Damage:" << damage << "HP:" << currentHP << "->" << hero->getHP();
            
            // ���»���Ѫ��
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �Ƴ��ӵ�
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
}

QPointF Bullet::getSceneCenter() const {
    // �ӵ�������(-10, -10, 20, 20)���������ĵ���Ҫ����ƫ����
    // ���ε����ĵ������itemԭ���ƫ����(0, 0)����Ϊ��������(-10, -10)Ϊ���Ͻ�
    return scenePos();
}
