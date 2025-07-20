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
    setPos(-80, -140); // ����λ��
    setRotation(0);
    setZValue(2);
}

void Ball::advance(int phase) {
    if (phase == 0) return;
    // ����ѱ�ǲ���Ծ��ֹͣ�˶�
    if (!active) return;
    
    // ֻ����δ������ʱ�ż������ʱ��
    if (!isMounted()) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - creationTime > maxLifetime) {
            if (scene()) {
                this->deleteLater();
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
        qDebug() << "[DEBUG] Ball gravity applied: vy =" << velocity.y() << "gravity =" << currentGravity << "isThrown =" << isThrown;
    }
    gravityDebugCounter++;
    
    // ����λ��
    QPointF oldPos = scenePos();
    moveBy(velocity.x(), velocity.y());
    QPointF newPos = scenePos();
    
    // �������
    static int debugCounter = 0;
    if (debugCounter % 30 == 0) {
        qDebug() << "[DEBUG] Ball pos:" << newPos << "velocity:" << velocity;
    }
    debugCounter++;
    
    // ����Ƿ��䵽����򳬳��߽�
    if (scene()) {
        BattleScene* battleScene = qobject_cast<BattleScene*>(scene());
        if (battleScene) {
            // ��ȡ����߶�
            qreal groundY = battleScene->getGroundHeight();
            if (newPos.y() >= groundY) {
                qDebug() << "[DEBUG] Ball hit ground, removing";
                // ֻ�е����ǵ�������ʱ�Ŵ� BattleScene ���б����Ƴ�
                // Ͷ�������� fallingWeapons �б��У�����Ҫ�Ƴ�
                if (!shooter) {
                    QMetaObject::invokeMethod(battleScene, "removeFallingWeapon", Q_ARG(Weapon*, this));
                }
                this->deleteLater();
                return;
            }
        }
        
        // ����Ƿ񳬳���Ļ�߽�
        qreal currentX = scenePos().x();
        qreal currentY = scenePos().y();
        if (currentX < -50 || currentX > 1330 || currentY > 1000) {
            qDebug() << "[DEBUG] Ball removed (off screen)";
            this->deleteLater();
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
            qDebug() << "[DEBUG] Ball hit character! Dealing 15 damage";
            int currentHP = character->getHP();
            character->setHP(std::max(0, currentHP - 15));
            
            // ���»���Ѫ��
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �Ƴ�Ǧ��
            this->deleteLater();
            return;
        }
    }
    
    // �����Hero����ײ
    Character* hero = battleScene->getHero();
    if (hero && hero != shooter) {
        if (hero->checkBulletCollision(ballPos)) {
            qDebug() << "[DEBUG] Ball hit hero! Dealing 15 damage";
            int currentHP = hero->getHP();
            hero->setHP(std::max(0, currentHP - 50));
            
            // ���»���Ѫ��
            scene()->invalidate(scene()->sceneRect(), QGraphicsScene::ForegroundLayer);
            
            // �Ƴ�Ǧ��
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
