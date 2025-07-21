#include "BattleScene.h"
#include <QDateTime>
#include <QTimer>
#include "../Items/Weapons/Bullet.h"
#include <QPainter>
void BattleScene::drawForeground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);

    int barWidth = 200;
    int barHeight = 24;
    int margin = 16;
    int hpMax = 100;
    // Character HP
    if (character) {
        int hp = character->getHP();
        QRect bgRect(margin, margin, barWidth, barHeight);
        QRect fgRect(margin, margin, barWidth * std::max(0, std::min(hp, hpMax)) / hpMax, barHeight);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(60, 60, 60, 200));
        painter->drawRect(bgRect);
        painter->setBrush(QColor(220, 40, 40, 220));
        painter->drawRect(fgRect);
        painter->setPen(Qt::white);
        painter->drawText(bgRect, Qt::AlignCenter, QString("Character HP: %1").arg(hp));
    }
    // Hero HP
    if (hero) {
        int hp = hero->getHP();
        QRect bgRect(sceneRect().width() - barWidth - margin, margin, barWidth, barHeight);
        QRect fgRect(sceneRect().width() - barWidth - margin, margin, barWidth * std::max(0, std::min(hp, hpMax)) / hpMax, barHeight);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(60, 60, 60, 200));
        painter->drawRect(bgRect);
        painter->setBrush(QColor(40, 120, 220, 220));
        painter->drawRect(fgRect);
        painter->setPen(Qt::white);
        painter->drawText(bgRect, Qt::AlignCenter, QString("Hero HP: %1").arg(hp));
    }
}

#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Characters/Hero.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"
#include "../Items/Weapons/Pistol.h"
#include "../Items/Weapons/Shotgun.h"
#include "../Items/Weapons/Submachine.h"
#include "../Items/Weapons/Knife.h"
#include "../Items/Weapons/Ball.h"

BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 1280, 720);
    map = new Battlefield();
    character = new Link();
    spareArmor = new FlamebreakerArmor();
    addItem(map);
    addItem(character);
    addItem(spareArmor);
    map->scaleToFitScene(this);
    character->setPos(map->getSpawnPos());
    character->setGroundY(map->getFloorHeight());
    spareArmor->unmount();
    spareArmor->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) * 0.75, map->getFloorHeight());
    
    // Create second character Hero
    hero = new Hero();
    addItem(hero);
    hero->setPos(map->getSpawnPos() + QPointF(400, 0)); // A bit to the right
    hero->setGroundY(map->getFloorHeight());
    

    // every 10 seconds, spawn a random weapon
    weaponDropTimer = new QTimer(this);
    connect(weaponDropTimer, &QTimer::timeout, this, &BattleScene::spawnRandomWeapon);
    weaponDropTimer->start(10000);

    // ÿ8�����һ�����ҩƷ
    medicineDropTimer = new QTimer(this);
    connect(medicineDropTimer, &QTimer::timeout, this, &BattleScene::spawnRandomMedicine);
    medicineDropTimer->start(8000);

    // ÿ�����һ�������ӵ��ľ���λ��
    bulletDebugTimer = new QTimer(this);
    connect(bulletDebugTimer, &QTimer::timeout, this, &BattleScene::debugAllBulletPositions);
    bulletDebugTimer->start(1000);

    // ÿ5�����һ����ײ����ζ���
    hitBoxDebugTimer = new QTimer(this);
    connect(hitBoxDebugTimer, &QTimer::timeout, this, &BattleScene::debugHitBoxCorners);
    hitBoxDebugTimer->start(5000);
}

void BattleScene::debugAllBulletPositions() {
    // qDebug() << "[DEBUG] === Bullet Debug Info ===";
    for (QGraphicsItem *item : items()) {
        auto bullet = dynamic_cast<Bullet *>(item);
        if (bullet) {
            // qDebug() << "[DEBUG] Bullet pos():" << bullet->pos() << "scenePos():" << bullet->scenePos() << "center:" << bullet->getSceneCenter();
        }
    }
    // qDebug() << "[DEBUG] === End Bullet Debug ===";
}

void BattleScene::spawnRandomWeapon() {
    int type = QRandomGenerator::global()->bounded(5);
    Weapon *weapon = nullptr;
    if (type == 0) weapon = new Pistol(nullptr);
    else if (type == 1) weapon = new Shotgun(nullptr);
    else if (type == 2) weapon = new Submachine(nullptr);
    else if (type == 3) weapon = new Knife(nullptr);
    else weapon = new Ball(nullptr);
    weapon->unmount();
    qreal randomX = sceneRect().left() + 100 + QRandomGenerator::global()->bounded((int)sceneRect().width() - 200);
    weapon->setPos(randomX, sceneRect().top());
    addItem(weapon);
    fallingWeapons.append(qMakePair(weapon, QDateTime::currentMSecsSinceEpoch()));
}

void BattleScene::spawnRandomMedicine() {
    int type = QRandomGenerator::global()->bounded(3);
    Medicine *medicine = nullptr;
    if (type == 0) medicine = new Bandage(nullptr);
    else if (type == 1) medicine = new Medkit(nullptr);
    else medicine = new Adrenaline(nullptr);
    
    medicine->unmount();
    qreal randomX = sceneRect().left() + 100 + QRandomGenerator::global()->bounded((int)sceneRect().width() - 200);
    medicine->setPos(randomX, sceneRect().top());
    addItem(medicine);
    fallingMedicines.append(qMakePair(medicine, QDateTime::currentMSecsSinceEpoch()));
    
    // qDebug() << "[DEBUG] Spawned medicine:" << medicine->getMedicineName() << "at position:" << medicine->pos();
}

// Update gravity
void BattleScene::updateFallingWeapons() {
    const qreal gravity = 0.5; // gravity
    const qreal maxY = map->getFloorHeight();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<int> toRemove;
    QList<Weapon*> toDelete;
    for (int i = 0; i < fallingWeapons.size(); ++i) {
        Weapon *weapon = fallingWeapons[i].first;
        // If the weapon is a Ball, its movement is handled by its own advance() method.
        if (dynamic_cast<Ball*>(weapon)) {
            continue;
        }
        qint64 born = fallingWeapons[i].second;
        // 20sec duration��if unmounted, remove
        if (now - born > 20000 && !weapon->isMounted()) {
            removeItem(weapon);
            toDelete.append(weapon);
            toRemove.append(i);
            continue;
        }
        // drop
        QPointF pos = weapon->pos();
        if (pos.y() < maxY) {
            weapon->setPos(pos.x(), std::min(maxY, pos.y() + gravity * 16));
        }
    }
    // clear the removed items
    for (int i = toRemove.size() - 1; i >= 0; --i) {
        fallingWeapons.removeAt(toRemove[i]);
    }
    for (Weapon* w : toDelete) {
        delete w;
    }
}

// ���µ����ҩƷ
void BattleScene::updateFallingMedicines() {
    const qreal gravity = 0.5; // �������ٶ�
    const qreal maxY = map->getFloorHeight();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<int> toRemove;
    QList<Medicine*> toDelete;
    
    for (int i = 0; i < fallingMedicines.size(); ++i) {
        Medicine *medicine = fallingMedicines[i].first;
        qint64 born = fallingMedicines[i].second;
        
        // 10������ʱ�䣬���δ��װ����ɾ��
        if (now - born > 10000 && !medicine->isMounted()) {
            removeItem(medicine);
            toDelete.append(medicine);
            toRemove.append(i);
            // qDebug() << "[DEBUG] Medicine" << medicine->getMedicineName() << "expired after 10 seconds";
            continue;
        }
        
        // ��������
        QPointF pos = medicine->pos();
        if (pos.y() < maxY) {
            medicine->setPos(pos.x(), std::min(maxY, pos.y() + gravity * 16));
        }
    }
    
    // �������Ƴ�����Ŀ
    for (int i = toRemove.size() - 1; i >= 0; --i) {
        fallingMedicines.removeAt(toRemove[i]);
    }
    for (Medicine* m : toDelete) {
        delete m;
    }
}

void BattleScene::processInput() {
    Scene::processInput();
    if (character != nullptr) character->processInput();
    if (hero != nullptr) hero->processInput();
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_G:
            // qDebug() << "[DEBUG] G key pressed";
            // character����
            if (character != nullptr) {
                // qDebug() << "[DEBUG] Character exists";
                
                // ��鹥����ȴʱ��
                if (!character->canAttack()) {
                    // qDebug() << "[DEBUG] Character attack on cooldown, ignoring";
                    break;
                }
                
                if (character->getWeapon() != nullptr) {
                    // qDebug() << "[DEBUG] Character has weapon";
                    Weapon *weapon = character->getWeapon();
                    
                    // ���������������ù�����ȴʱ��
                    if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                        character->setAttackCooldown(pistol->getFireRate());
                        // qDebug() << "[DEBUG] Set pistol fire rate:" << pistol->getFireRate() << "ms";
                    } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                        character->setAttackCooldown(shotgun->getFireRate());
                        // qDebug() << "[DEBUG] Set shotgun fire rate:" << shotgun->getFireRate() << "ms";
                    } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                        character->setAttackCooldown(submachine->getFireRate());
                        // qDebug() << "[DEBUG] Set submachine fire rate:" << submachine->getFireRate() << "ms";
                    } else {
                        // ��������ʹ��Ĭ����ȴʱ��
                        character->setAttackCooldown(500);
                        // qDebug() << "[DEBUG] Set default fire rate: 500ms";
                    }
                } else {
                    // ȭͷ����ʹ��Ĭ����ȴʱ��
                    character->setAttackCooldown(500);
                    // qDebug() << "[DEBUG] Set fist attack fire rate: 500ms";
                }
                
                // ��ʼ������ȴ��ʱ
                character->startAttackCooldown();
                
                if (character->getWeapon() != nullptr) {
                    // qDebug() << "[DEBUG] Character has weapon";
                    Weapon *weapon = character->getWeapon();
                    
                    // ����Ƿ�ΪС��
                    if (weapon->getWeaponName() == "Knife") {
                        // qDebug() << "[DEBUG] Character using knife attack";
                        // С�������߼������С��λ���Ƿ��ڶԷ���ײ����
                        QPointF charPos = character->scenePos();
                        QPointF knifePos;
                        
                        // С������λ��Ӧ����ȭͷλ��һ��
                        // ���ݽ�ɫ����ȷ��С��λ��
                        if (character->isFacingRight()) {
                            knifePos = QPointF(charPos.x() + 33, charPos.y() - 85);
                        } else {
                            knifePos = QPointF(charPos.x() - 33, charPos.y() - 85);
                        }
                        // qDebug() << "[DEBUG] Knife position:" << knifePos;
                        
                        // ����Ƿ����Hero
                        if (hero && hero->isHitByPoint(knifePos)) {
                            // qDebug() << "[DEBUG] Knife hit Hero! Dealing 10 damage";
                            int newHP = std::max(0, hero->getHP() - 10); // ȷ��HP������0
                            hero->setHP(newHP);
                            // qDebug() << "[DEBUG] Hero HP after knife attack:" << hero->getHP();
                            invalidate(sceneRect(), QGraphicsScene::ForegroundLayer); // ǿ���ػ�ǰ����
                        } else {
                            // qDebug() << "[DEBUG] Knife attack missed";
                        }
                    }
                    else if (weapon->getWeaponName() == "Ball") {
                        // qDebug() << "[DEBUG] Character using ball attack";
                        // Ǧ�򹥻��߼���Ͷ��һ���µ�Ball����
                        if (weapon->getAmmo() > 0) {
                            // qDebug() << "[DEBUG] Ball used, creating thrown ball";
                            
                            // ����һ���µ�Ball��������Ͷ��
                            Ball* thrownBall = new Ball(nullptr);
                            thrownBall->unmount(); // ȷ�����ǹ���״̬
                            
                            // ����Ͷ��Ball�ĳ�ʼλ�ã���ɫλ�ã�
                            QPointF charPos = character->scenePos();
                            thrownBall->setPos(charPos.x(), charPos.y() - 100); // �ӽ�ɫ�Ϸ�Ͷ��
                            
                            // ����Ͷ��Ball�ĳ�ʼ�ٶ�
                            qreal vx = character->isFacingRight() ? 15 : -15; // x���ٶȸ��ݳ���
                            qreal vy = -25; // y���ٶ����ϣ���ʼ�ٶȸ���
                            thrownBall->setVelocity(vx, vy);
                            thrownBall->setThrownMode(true); // ����ΪͶ��ģʽ��ʹ�ø��������
                            thrownBall->shooter = character; // ��������
                            
                            // ��ӵ�������
                            addItem(thrownBall);
                            // qDebug() << "[DEBUG] Thrown ball created at pos:" << thrownBall->pos() << "with velocity:" << vx << "," << vy;
                            
                            // ԭ�����������߼�
                            // qDebug() << "[DEBUG] Step 1: About to call removeFallingWeapon";
                            
                            // �����������ʧ
                            // �ȴ� fallingWeapons �б����Ƴ����ٰ�ȫɾ��
                            removeFallingWeapon(weapon);
                            // qDebug() << "[DEBUG] Step 2: removeFallingWeapon completed";
                            
                            removeItem(weapon);
                            // qDebug() << "[DEBUG] Step 3: removeItem completed";
                            
                            // qDebug() << "[DEBUG] Step 4a: About to call character->pickupWeapon(nullptr)";
                            // qDebug() << "[DEBUG] Step 4b: character pointer is" << character;
                            character->pickupWeapon(nullptr);
                            // qDebug() << "[DEBUG] Step 4: pickupWeapon(nullptr) completed";
                            
                            // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
                            // qDebug() << "[DEBUG] Step 5: About to schedule weapon deletion";
                            QTimer::singleShot(0, [weapon]() {
                                // qDebug() << "[DEBUG] Lambda: About to delete weapon";
                                delete weapon;
                                // qDebug() << "[DEBUG] Lambda: Weapon deleted successfully";
                            });
                            // qDebug() << "[DEBUG] Step 6: Weapon deletion scheduled";
                            
                            // qDebug() << "[DEBUG] Character is now empty-handed";
                        } else {
                            // qDebug() << "[DEBUG] Ball has no ammo left";
                        }
                    } else {
                        // Զ�����������߼�
                        // qDebug() << "[DEBUG] Weapon ammo:" << weapon->getAmmo();
                        if (weapon->getAmmo() > 0) {
                            weapon->decAmmo();
                            QPointF charPos = character->pos();
                            QPointF gunPos = weapon->scenePos();
                            // qDebug() << "[DEBUG] Character pos:" << charPos;
                            // qDebug() << "[DEBUG] Weapon scene pos:" << gunPos;
                            // qDebug() << "[DEBUG] Character facing right:" << character->isFacingRight();
                            
                            // �����������ͻ�ȡ��ͬ������
                            qreal bulletSpeed = 22.5; // Ĭ���ٶ�
                            int bulletDamage = 20;     // Ĭ���˺�
                            
                            if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                                bulletSpeed = pistol->getBulletSpeed();
                                bulletDamage = pistol->getBulletDamage();
                                // qDebug() << "[DEBUG] Using Pistol - Speed:" << bulletSpeed << "Damage:" << bulletDamage;
                            } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                                bulletSpeed = shotgun->getBulletSpeed();
                                bulletDamage = shotgun->getBulletDamage();
                                // qDebug() << "[DEBUG] Using Shotgun - Speed:" << bulletSpeed << "Damage:" << bulletDamage;
                            } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                                bulletSpeed = submachine->getBulletSpeed();
                                bulletDamage = submachine->getBulletDamage();
                                // qDebug() << "[DEBUG] Using Submachine - Speed:" << bulletSpeed << "Damage:" << bulletDamage;
                            }
                            
                            qreal vx = character->isFacingRight() ? bulletSpeed : -bulletSpeed;
                            qreal bx = gunPos.x();
                            qreal by = gunPos.y();
                            
                            // qDebug() << "[DEBUG] Bullet spawn pos:" << bx << "," << by;
                            // qDebug() << "[DEBUG] Bullet velocity:" << vx;
                            
                            Bullet *bullet = new Bullet(bx, by, vx, bulletDamage);
                            bullet->shooter = character;
                            bullet->setZValue(100);
                            addItem(bullet);
                            // qDebug() << "[DEBUG] Bullet created and added to scene";
                        } else {
                            // qDebug() << "[DEBUG] No ammo left";
                        }
                    }
                } else {
                    // qDebug() << "[DEBUG] Character has no weapon - using fist attack";
                    // ȭͷ�����߼�������ɫ����λ���Ƿ��е���
                    QPointF charPos = character->scenePos();
                    QPointF fistPos;
                    
                    // ȭͷ����λ��Ӧ��������λ��һ��
                    // ���ݽ�ɫ����ȷ��ȭͷλ��
                    if (character->isFacingRight()) {
                        fistPos = QPointF(charPos.x() + 33, charPos.y() - 85);
                    } else {
                        fistPos = QPointF(charPos.x() - 33, charPos.y() - 85);
                    }
                    
                    qDebug() << "[DEBUG] Fist attack position:" << fistPos;
                    
                    // ����Ƿ����Hero
                    if (hero && hero->isHitByPoint(fistPos)) {
                        qDebug() << "[DEBUG] Fist hit Hero! Dealing 3 damage";
                        int newHP = std::max(0, hero->getHP() - 3); // ȷ��HP������0
                        hero->setHP(newHP);
                        // qDebug() << "[DEBUG] Hero HP after fist attack:" << hero->getHP();
                        invalidate(sceneRect(), QGraphicsScene::ForegroundLayer); // ǿ���ػ�ǰ����
                    } else {
                        qDebug() << "[DEBUG] Fist attack missed";
                    }
                }
            } else {
                // qDebug() << "[DEBUG] Character is null";
            }
            break;
        case Qt::Key_K:
            // qDebug() << "[DEBUG] K key pressed";
            // hero����
            if (hero != nullptr) {
                // qDebug() << "[DEBUG] Hero exists";
                
                // ��鹥����ȴʱ��
                if (!hero->canAttack()) {
                    // qDebug() << "[DEBUG] Hero attack on cooldown, ignoring";
                    break;
                }
                
                if (hero->getWeapon() != nullptr) {
                    // qDebug() << "[DEBUG] Hero has weapon";
                    Weapon *weapon = hero->getWeapon();
                    
                    // ���������������ù�����ȴʱ��
                    if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                        hero->setAttackCooldown(pistol->getFireRate());
                        // qDebug() << "[DEBUG] Set hero pistol fire rate:" << pistol->getFireRate() << "ms";
                    } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                        hero->setAttackCooldown(shotgun->getFireRate());
                        // qDebug() << "[DEBUG] Set hero shotgun fire rate:" << shotgun->getFireRate() << "ms";
                    } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                        hero->setAttackCooldown(submachine->getFireRate());
                        // qDebug() << "[DEBUG] Set hero submachine fire rate:" << submachine->getFireRate() << "ms";
                    } else {
                        // ��������ʹ��Ĭ����ȴʱ��
                        hero->setAttackCooldown(500);
                        // qDebug() << "[DEBUG] Set hero default fire rate: 500ms";
                    }
                } else {
                    // ȭͷ����ʹ��Ĭ����ȴʱ��
                    hero->setAttackCooldown(500);
                    // qDebug() << "[DEBUG] Set hero fist attack fire rate: 500ms";
                }
                
                // ��ʼ������ȴ��ʱ
                hero->startAttackCooldown();
                
                if (hero->getWeapon() != nullptr) {
                    // qDebug() << "[DEBUG] Hero has weapon";
                    Weapon *weapon = hero->getWeapon();
                    
                    // ����Ƿ�ΪС��
                    if (weapon->getWeaponName() == "Knife") {
                        // qDebug() << "[DEBUG] Hero using knife attack";
                        // С�������߼������С��λ���Ƿ��ڶԷ���ײ����
                        QPointF heroPos = hero->scenePos();
                        QPointF knifePos;
                        
                        // С������λ��Ӧ����ȭͷλ��һ��
                        // ����Ӣ�۳���ȷ��С��λ��
                        if (hero->isFacingRight()) {
                            knifePos = QPointF(heroPos.x() + 33, heroPos.y() - 85);
                        } else {
                            knifePos = QPointF(heroPos.x() - 33, heroPos.y() - 85);
                        }
                        // qDebug() << "[DEBUG] Knife position:" << knifePos;
                        
                        // ����Ƿ����Character
                        if (character && character->isHitByPoint(knifePos)) {
                            // qDebug() << "[DEBUG] Knife hit Character! Dealing 10 damage";
                            int newHP = std::max(0, character->getHP() - 10); // ȷ��HP������0
                            character->setHP(newHP);
                            // qDebug() << "[DEBUG] Character HP after knife attack:" << character->getHP();
                            invalidate(sceneRect(), QGraphicsScene::ForegroundLayer); // ǿ���ػ�ǰ����
                        } else {
                            // qDebug() << "[DEBUG] Knife attack missed";
                        }
                    } else if (weapon->getWeaponName() == "Ball") {
                        // qDebug() << "[DEBUG] Hero using ball attack";
                        // Ǧ�򹥻��߼���Ͷ��һ���µ�Ball����
                        if (weapon->getAmmo() > 0) {
                            // qDebug() << "[DEBUG] Ball used, creating thrown ball";
                            
                            // ����һ���µ�Ball��������Ͷ��
                            Ball* thrownBall = new Ball(nullptr);
                            thrownBall->unmount(); // ȷ�����ǹ���״̬
                            
                            // ����Ͷ��Ball�ĳ�ʼλ�ã�Ӣ��λ�ã�
                            QPointF heroPos = hero->scenePos();
                            thrownBall->setPos(heroPos.x(), heroPos.y() - 100); // ��Ӣ���Ϸ�Ͷ��
                            
                            // ����Ͷ��Ball�ĳ�ʼ�ٶ�
                            qreal vx = hero->isFacingRight() ? 15 : -15; // x���ٶȸ��ݳ���
                            qreal vy = -25; // y���ٶ����ϣ���ʼ�ٶȸ���
                            thrownBall->setVelocity(vx, vy);
                            thrownBall->setThrownMode(true); // ����ΪͶ��ģʽ��ʹ�ø��������
                            thrownBall->shooter = hero; // ��������
                            
                            // ��ӵ�������
                            addItem(thrownBall);
                            // qDebug() << "[DEBUG] Thrown ball created at pos:" << thrownBall->pos() << "with velocity:" << vx << "," << vy;
                            
                            // ԭ�����������߼�
                            // qDebug() << "[DEBUG] Step 1: About to call removeFallingWeapon";
                            
                            // �����������ʧ
                            // �ȴ� fallingWeapons �б����Ƴ����ٰ�ȫɾ��
                            removeFallingWeapon(weapon);
                            // qDebug() << "[DEBUG] Step 2: removeFallingWeapon completed";
                            
                            removeItem(weapon);
                            // qDebug() << "[DEBUG] Step 3: removeItem completed";
                            
                            // qDebug() << "[DEBUG] Step 4a: About to call hero->pickupWeapon(nullptr)";
                            // qDebug() << "[DEBUG] Step 4b: hero pointer is" << hero;
                            hero->pickupWeapon(nullptr);
                            // qDebug() << "[DEBUG] Step 4: pickupWeapon(nullptr) completed";
                            
                            // ʹ�ö�ʱ���ӳ�ɾ������������ɾ�����µ�����
                            // qDebug() << "[DEBUG] Step 5: About to schedule weapon deletion";
                            QTimer::singleShot(0, [weapon]() {
                                // qDebug() << "[DEBUG] Lambda: About to delete weapon";
                                delete weapon;
                                // qDebug() << "[DEBUG] Lambda: Weapon deleted successfully";
                            });
                            // qDebug() << "[DEBUG] Step 6: Weapon deletion scheduled";
                            
                            // qDebug() << "[DEBUG] Hero is now empty-handed";
                        } else {
                            // qDebug() << "[DEBUG] Ball has no ammo left";
                        }
                    } else {
                        // Զ�����������߼�
                        // qDebug() << "[DEBUG] Weapon ammo:" << weapon->getAmmo();
                        if (weapon->getAmmo() > 0) {
                            weapon->decAmmo();
                            QPointF heroPos = hero->pos();
                            QPointF gunPos = weapon->scenePos();
                            // qDebug() << "[DEBUG] Hero pos:" << heroPos;
                            // qDebug() << "[DEBUG] Weapon scene pos:" << gunPos;
                            // qDebug() << "[DEBUG] Hero facing right:" << hero->isFacingRight();

                            // �����������ͻ�ȡ��ͬ������
                            qreal bulletSpeed = 22.5; // Ĭ���ٶ�
                            int bulletDamage = 20;     // Ĭ���˺�
                            
                            if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                                bulletSpeed = pistol->getBulletSpeed();
                                bulletDamage = pistol->getBulletDamage();
                                // qDebug() << "[DEBUG] Using Pistol - Speed:" << bulletSpeed << "Damage:" << bulletDamage;
                            } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                                bulletSpeed = shotgun->getBulletSpeed();
                                bulletDamage = shotgun->getBulletDamage();
                                // qDebug() << "[DEBUG] Using Shotgun - Speed:" << bulletSpeed << "Damage:" << bulletDamage;
                            } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                                bulletSpeed = submachine->getBulletSpeed();
                                bulletDamage = submachine->getBulletDamage();
                                // qDebug() << "[DEBUG] Using Submachine - Speed:" << bulletSpeed << "Damage:" << bulletDamage;
                            }

                            qreal vx = hero->isFacingRight() ? bulletSpeed : -bulletSpeed;
                            qreal bx = gunPos.x();
                            qreal by = gunPos.y();

                            // qDebug() << "[DEBUG] Bullet spawn pos:" << bx << "," << by;
                            // qDebug() << "[DEBUG] Bullet velocity:" << vx;

                            Bullet *bullet = new Bullet(bx, by, vx, bulletDamage);
                            bullet->shooter = hero;
                            bullet->setZValue(100);
                            addItem(bullet);
                            // qDebug() << "[DEBUG] Bullet created and added to scene";
                        } else {
                            // qDebug() << "[DEBUG] No ammo left";
                        }
                    }
                } else {
                    // ȭͷ�����߼������Ӣ������λ���Ƿ��е���
                    QPointF heroPos = hero->scenePos();
                    QPointF fistPos;
                    
                    // ȭͷ����λ��Ӧ��������λ��һ��
                    // ����Ӣ�۳���ȷ��ȭͷλ��
                    if (hero->isFacingRight()) {
                        fistPos = QPointF(heroPos.x() + 33, heroPos.y() - 85);
                    } else {
                        fistPos = QPointF(heroPos.x() - 33, heroPos.y() - 85);
                    }
                    
                    // ����Ƿ����Character
                    if (character && character->isHitByPoint(fistPos)) {
                        int newHP = std::max(0, character->getHP() - 3); // ȷ��HP������0
                        character->setHP(newHP);
                        invalidate(sceneRect(), QGraphicsScene::ForegroundLayer); // ǿ���ػ�ǰ����
                    }
                }
            } else {
                qDebug() << "[DEBUG] Hero is null";
            }
            break;
        case Qt::Key_A:
            if (character != nullptr) character->setLeftDown(true);
            break;
        case Qt::Key_D:
            if (character != nullptr) character->setRightDown(true);
            break;
        case Qt::Key_F:
            if (character != nullptr) character->setPickDown(true);
            break;
        case Qt::Key_W:
            if (character != nullptr) character->jump();
            break;
        case Qt::Key_S:
            if (character != nullptr) character->setCrouchDown(true);
            break;
        case Qt::Key_Left:
            if (hero != nullptr) hero->setLeftDown(true);
            break;
        case Qt::Key_Right:
            if (hero != nullptr) hero->setRightDown(true);
            break;
        case Qt::Key_Up:
            if (hero != nullptr) hero->jump();
            break;
        case Qt::Key_Down:
            if (hero != nullptr) hero->setCrouchDown(true);
            break;
        case Qt::Key_L:
            if (hero != nullptr) hero->setPickDown(true);
            break;
        default:
            Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_A:
            if (character != nullptr) character->setLeftDown(false);
            break;
        case Qt::Key_D:
            if (character != nullptr) character->setRightDown(false);
            break;
        case Qt::Key_F:
            if (character != nullptr) character->setPickDown(false);
            break;
        case Qt::Key_S:
            if (character != nullptr) character->setCrouchDown(false);
            break;
        case Qt::Key_Left:
            if (hero != nullptr) hero->setLeftDown(false);
            break;
        case Qt::Key_Right:
            if (hero != nullptr) hero->setRightDown(false);
            break;
        case Qt::Key_Down:
            if (hero != nullptr) hero->setCrouchDown(false);
            break;
        case Qt::Key_L:
            if (hero != nullptr) hero->setPickDown(false);
            break;
        default:
            Scene::keyReleaseEvent(event);
    }
}

void BattleScene::update() {
    Scene::update();
    updateFallingWeapons();
    updateFallingMedicines();
    
    // ÿ10֡���»���Ѫ����
    frameCounter++;
    if (frameCounter >= 10) {
        frameCounter = 0;
        // ǿ���ػ�ǰ����Ѫ������drawForeground�л��ƣ�
        invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
    }
    
    advance();
}

void BattleScene::processMovement() {
    Scene::processMovement();
    if (character != nullptr) {
        character->applyGravity(deltaTime);
        character->setPos(character->pos() + character->getVelocity() * (double) deltaTime);
    }
    if (hero != nullptr) {
        hero->applyGravity(deltaTime);
        hero->setPos(hero->pos() + hero->getVelocity() * (double) deltaTime);
    }
}

void BattleScene::processPicking() {
    Scene::processPicking();
    if (character && character->isPicking()) {
        auto mountable = findNearestUnmountedMountable(character->pos(), 100.);
        if (mountable != nullptr) {
            auto oldItem = pickupMountable(character, mountable);
            if (auto oldArmor = dynamic_cast<Armor *>(oldItem)) {
                spareArmor = oldArmor;
            }
        }
        
        // ��鸽����ҩƷ
        auto medicine = findNearestMedicine(character->pos(), 100.);
        if (medicine != nullptr) {
            character->pickupMedicine(medicine);
            // �ӵ���ҩƷ�б����Ƴ�
            for (int i = 0; i < fallingMedicines.size(); ++i) {
                if (fallingMedicines[i].first == medicine) {
                    fallingMedicines.removeAt(i);
                    break;
                }
            }
            removeItem(medicine);
            delete medicine;
        }
    }
    if (hero && hero->isPicking()) {
        auto mountable = findNearestUnmountedMountable(hero->pos(), 100.);
        if (mountable != nullptr) {
            pickupMountable(hero, mountable);
        }
        
        // HeroҲ����ʰȡҩƷ
        auto medicine = findNearestMedicine(hero->pos(), 100.);
        if (medicine != nullptr) {
            hero->pickupMedicine(medicine);
            // �ӵ���ҩƷ�б����Ƴ�
            for (int i = 0; i < fallingMedicines.size(); ++i) {
                if (fallingMedicines[i].first == medicine) {
                    fallingMedicines.removeAt(i);
                    break;
                }
            }
            removeItem(medicine);
            delete medicine;
        }
    }
}

Mountable *BattleScene::findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold) {
    Mountable *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto mountable = dynamic_cast<Mountable *>(item)) {
            if (!mountable->isMounted()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = mountable;
                }
            }
        }
    }

    return nearest;
}

Medicine *BattleScene::findNearestMedicine(const QPointF &pos, qreal distance_threshold) {
    Medicine *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto medicine = dynamic_cast<Medicine *>(item)) {
            if (!medicine->isMounted()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = medicine;
                }
            }
        }
    }

    return nearest;
}

Mountable *BattleScene::pickupMountable(Character *character, Mountable *mountable) {
    // Support for armor and weapons
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        return character->pickupArmor(armor);
    } else if (auto weapon = dynamic_cast<Weapon *>(mountable)) {
        auto oldWeapon = character->pickupWeapon(weapon);
        
        // ����о��������滻���䣬������ʣ��ʱ��Ϊ0.5��
        if (oldWeapon) {
            qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
            qint64 shortLifeTime = currentTime - 19500; // 20000 - 500 = 19500, ʣ��0.5��
            
            // ���� fallingWeapons �б��и�������ʱ���
            for (int i = 0; i < fallingWeapons.size(); ++i) {
                if (fallingWeapons[i].first == oldWeapon) {
                    fallingWeapons[i].second = shortLifeTime;
                    // qDebug() << "[DEBUG] Set dropped weapon remaining time to 0.5 seconds";
                    break;
                }
            }
            
            // ������������� fallingWeapons �б��У������������Ϊ���������ڣ�
            bool found = false;
            for (const auto& pair : fallingWeapons) {
                if (pair.first == oldWeapon) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                fallingWeapons.append(qMakePair(oldWeapon, shortLifeTime));
                // qDebug() << "[DEBUG] Added dropped weapon to fallingWeapons with 0.5s remaining time";
            }
        }
        
        return oldWeapon;
    }
    return nullptr;
}

void BattleScene::generateRandomWeapons() {
    // Create one of each weapon type
    QVector<Weapon*> weapons;
    
    // Create each weapon
    weapons.append(new Pistol(nullptr));
    weapons.append(new Shotgun(nullptr));
    weapons.append(new Submachine(nullptr));
    weapons.append(new Knife(nullptr));
    weapons.append(new Ball(nullptr));
    
    // Shuffle weapon order
    std::random_shuffle(weapons.begin(), weapons.end());
    
    // Place weapons at random positions on the ground
    for (int i = 0; i < weapons.size(); ++i) {
        Weapon* weapon = weapons[i];
        weapon->unmount();
        
        // Random position on the ground, ensure some distance between weapons
        qreal randomX = sceneRect().left() + 100 + i * 200 + 
                       QRandomGenerator::global()->bounded(-50, 51);
        
        weapon->setPos(randomX, map->getFloorHeight());
        addItem(weapon);
    }
}

void BattleScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickPos = event->scenePos();
    qDebug() << "[DEBUG] Mouse clicked at absolute position:" << clickPos;
    
    // ���ø����������¼�����
    Scene::mousePressEvent(event);
}

void BattleScene::debugHitBoxCorners() {
    qDebug() << "[DEBUG] === HitBox Debug Info ===";
    
    // ���Character����ײ�䶥��
    if (character) {
        QRectF charHitBox = character->getHitBox();
        qDebug() << "[DEBUG] Character HitBox:";
        qDebug() << "  TopLeft:" << charHitBox.topLeft();
        qDebug() << "  TopRight:" << charHitBox.topRight();
        qDebug() << "  BottomLeft:" << charHitBox.bottomLeft();
        qDebug() << "  BottomRight:" << charHitBox.bottomRight();
        qDebug() << "  Center:" << charHitBox.center();
        qDebug() << "  Size:" << charHitBox.size();
        qDebug() << " Character Position: " << character->pos() ;
    }
    
    // ���Hero����ײ�䶥��
    if (hero) {
        QRectF heroHitBox = hero->getHitBox();
        qDebug() << "[DEBUG] Hero HitBox:";
        qDebug() << "  TopLeft:" << heroHitBox.topLeft();
        qDebug() << "  TopRight:" << heroHitBox.topRight();
        qDebug() << "  BottomLeft:" << heroHitBox.bottomLeft();
        qDebug() << "  BottomRight:" << heroHitBox.bottomRight();
        qDebug() << "  Center:" << heroHitBox.center();
        qDebug() << "  Size:" << heroHitBox.size();
    }
    
    qDebug() << "[DEBUG] === End HitBox Debug ===";
}

void BattleScene::removeFallingWeapon(Weapon* weapon) {
    qDebug() << "[DEBUG] removeFallingWeapon: Starting to remove weapon" << weapon;
    qDebug() << "[DEBUG] removeFallingWeapon: fallingWeapons.size() =" << fallingWeapons.size();
    
    bool found = false;
    for (int i = 0; i < fallingWeapons.size(); ++i) {
        if (fallingWeapons[i].first == weapon) {
            qDebug() << "[DEBUG] removeFallingWeapon: Found weapon at index" << i;
            fallingWeapons.removeAt(i);
            found = true;
            qDebug() << "[DEBUG] removeFallingWeapon: Weapon removed, new size =" << fallingWeapons.size();
            break;
        }
    }
    
    if (!found) {
        qDebug() << "[DEBUG] removeFallingWeapon: WARNING - Weapon not found in fallingWeapons list";
        // ��ӡ��������ָ����е���
        for (int i = 0; i < fallingWeapons.size(); ++i) {
            qDebug() << "[DEBUG] removeFallingWeapon: fallingWeapons[" << i << "] =" << fallingWeapons[i].first;
        }
    }
    
    qDebug() << "[DEBUG] removeFallingWeapon: Completed";
}
