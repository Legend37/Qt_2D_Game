#include "BattleScene.h"
#include <QDateTime>
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
}
void BattleScene::spawnRandomWeapon() {
    int type = QRandomGenerator::global()->bounded(3);
    Weapon *weapon = nullptr;
    if (type == 0) weapon = new Pistol(nullptr);
    else if (type == 1) weapon = new Shotgun(nullptr);
    else weapon = new Submachine(nullptr);
    weapon->unmount();
    qreal randomX = sceneRect().left() + 100 + QRandomGenerator::global()->bounded((int)sceneRect().width() - 200);
    weapon->setPos(randomX, sceneRect().top());
    addItem(weapon);
    fallingWeapons.append(qMakePair(weapon, QDateTime::currentMSecsSinceEpoch()));
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
        qint64 born = fallingWeapons[i].second;
        // 20sec duration，if unmounted, remove
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

void BattleScene::processInput() {
    Scene::processInput();
    if (character != nullptr) character->processInput();
    if (hero != nullptr) hero->processInput();
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_G:
            qDebug() << "[DEBUG] G key pressed";
            // character攻击，发射子弹
            if (character != nullptr) {
                qDebug() << "[DEBUG] Character exists";
                if (character->getWeapon() != nullptr) {
                    qDebug() << "[DEBUG] Character has weapon";
                    Weapon *weapon = character->getWeapon();
                    qDebug() << "[DEBUG] Weapon ammo:" << weapon->getAmmo();
                    if (weapon->getAmmo() > 0) {
                        weapon->decAmmo();
                        QPointF charPos = character->pos();
                        QPointF gunPos = weapon->scenePos(); // 使用scenePos()获取武器在场景中的绝对位置
                        qDebug() << "[DEBUG] Character pos:" << charPos;
                        qDebug() << "[DEBUG] Weapon scene pos:" << gunPos;
                        qDebug() << "[DEBUG] Character facing right:" << character->isFacingRight();
                        
                        qreal vx = character->isFacingRight() ? 22.5 : -22.5;

                        qreal bx = gunPos.x();
                        qreal by = gunPos.y();
                        
                        qDebug() << "[DEBUG] Bullet spawn pos:" << bx << "," << by;
                        qDebug() << "[DEBUG] Bullet velocity:" << vx;
                        
                        Bullet *bullet = new Bullet(bx, by, vx);
                        bullet->setZValue(100);
                        addItem(bullet);
                        qDebug() << "[DEBUG] Bullet created and added to scene";
                    } else {
                        qDebug() << "[DEBUG] No ammo left";
                    }
                } else {
                    qDebug() << "[DEBUG] Character has no weapon";
                }
            } else {
                qDebug() << "[DEBUG] Character is null";
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
    }
    if (hero && hero->isPicking()) {
        auto mountable = findNearestUnmountedMountable(hero->pos(), 100.);
        if (mountable != nullptr) {
            pickupMountable(hero, mountable);
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

Mountable *BattleScene::pickupMountable(Character *character, Mountable *mountable) {
    // Support for armor and weapons
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        return character->pickupArmor(armor);
    } else if (auto weapon = dynamic_cast<Weapon *>(mountable)) {
        return character->pickupWeapon(weapon);
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
