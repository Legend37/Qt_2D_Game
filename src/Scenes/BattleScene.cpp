#include "BattleScene.h"
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>
#include "../Items/Weapons/Bullet.h"
#include <QPainter>
void BattleScene::drawForeground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);
    int barWidth = 200;
    int barHeight = 24;
    int margin = 16;
    int hpMax = 100;
    int durabilityBarHeight = 16;
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
        painter->drawText(bgRect, Qt::AlignCenter, QString("Player 1 HP: %1").arg(hp));
        auto armor = character->getArmor();
        if (armor && armor->getDurability() > 0) {
            int durability = armor->getDurability();
            int maxDurability = armor->getMaxDurability();
            QRect durabilityBgRect(margin, margin + barHeight + 4, barWidth, durabilityBarHeight);
            QRect durabilityFgRect(margin, margin + barHeight + 4, 
                                   barWidth * std::max(0, std::min(durability, maxDurability)) / maxDurability, 
                                   durabilityBarHeight);
            painter->setBrush(QColor(60, 60, 60, 200));
            painter->drawRect(durabilityBgRect);
            painter->setBrush(QColor(100, 180, 255, 220));
            painter->drawRect(durabilityFgRect);
            painter->setPen(Qt::white);
            painter->setFont(QFont("Arial", 10));
            painter->drawText(durabilityBgRect, Qt::AlignCenter, QString("Armor: %1/%2").arg(durability).arg(maxDurability));
            painter->setFont(QFont());
        }
    }
    if (hero) {
        int hp = hero->getHP();
        QRect bgRect(sceneRect().width() - barWidth - margin, margin, barWidth, barHeight);
        QRect fgRect(sceneRect().width() - barWidth - margin, margin, barWidth * std::max(0, std::min(hp, hpMax)) / hpMax, barHeight);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(60, 60, 60, 200));
        painter->drawRect(bgRect);
        painter->setBrush(QColor(220, 40, 40, 220));
        painter->drawRect(fgRect);
        painter->setPen(Qt::white);
        painter->drawText(bgRect, Qt::AlignCenter, QString("Player 2 HP: %1").arg(hp));
        auto armor = hero->getArmor();
        if (armor && armor->getDurability() > 0) {
            int durability = armor->getDurability();
            int maxDurability = armor->getMaxDurability();
            QRect durabilityBgRect(sceneRect().width() - barWidth - margin, margin + barHeight + 4, barWidth, durabilityBarHeight);
            QRect durabilityFgRect(sceneRect().width() - barWidth - margin, margin + barHeight + 4,
                                   barWidth * std::max(0, std::min(durability, maxDurability)) / maxDurability,
                                   durabilityBarHeight);
            painter->setBrush(QColor(40, 40, 40, 200));
            painter->drawRect(durabilityBgRect);
            painter->setBrush(QColor(100, 180, 255, 220));
            painter->drawRect(durabilityFgRect);
            painter->setPen(Qt::white);
            painter->setFont(QFont("Arial", 10));
            painter->drawText(durabilityBgRect, Qt::AlignCenter, QString("Armor: %1/%2").arg(durability).arg(maxDurability));
            painter->setFont(QFont());
        }
    }
}
#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>
#include <random>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
#include "../Items/Characters/Hero.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Armors/FlamebreakerArmor.h"
#include "../Items/Armors/BodyArmor.h"
#include "../Items/Armors/OldShirt.h"
#include "../Items/Weapons/Pistol.h"
#include "../Items/Weapons/Shotgun.h"
#include "../Items/Weapons/Submachine.h"
#include "../Items/Weapons/Knife.h"
#include "../Items/Weapons/Ball.h"
BattleScene::BattleScene(QObject *parent) : Scene(parent) {
    setSceneRect(0, 0, 1280, 720);
    map = new Battlefield();
    character = new Link();
    addItem(map);
    addItem(character);
    map->scaleToFitScene(this);
    character->setPos(QPointF(100, map->getFloorHeight() - 100));
    character->setGroundY(map->getFloorHeight());
    hero = new Hero();
    addItem(hero);
    hero->setPos(QPointF(1100, map->getFloorHeight() - 100));
    hero->setGroundY(map->getFloorHeight());
    connect(character, &Character::characterDied, this, &BattleScene::onCharacterDied);
    connect(hero, &Character::characterDied, this, &BattleScene::onCharacterDied);
    weaponDropTimer = new QTimer(this);
    connect(weaponDropTimer, &QTimer::timeout, this, &BattleScene::spawnRandomWeapon);
    weaponDropTimer->start(10000);
    medicineDropTimer = new QTimer(this);
    connect(medicineDropTimer, &QTimer::timeout, this, &BattleScene::spawnRandomMedicine);
    medicineDropTimer->start(8000);
    armorDropTimer = new QTimer(this);
    connect(armorDropTimer, &QTimer::timeout, this, &BattleScene::spawnRandomArmor);
    armorDropTimer->start(20000);
    bulletDebugTimer = new QTimer(this);
    connect(bulletDebugTimer, &QTimer::timeout, this, &BattleScene::debugAllBulletPositions);
    bulletDebugTimer->start(1000);
    hitBoxDebugTimer = new QTimer(this);
    connect(hitBoxDebugTimer, &QTimer::timeout, this, &BattleScene::debugHitBoxCorners);
    hitBoxDebugTimer->start(5000);
}
void BattleScene::debugAllBulletPositions() {
    for (QGraphicsItem *item : items()) {
        auto bullet = dynamic_cast<Bullet *>(item);
        if (bullet) {
        }
    }
}
void BattleScene::spawnRandomWeapon() {
    int type = QRandomGenerator::global()->bounded(5);
    Weapon *weapon = nullptr;
    if (type == 0) {
        weapon = new Pistol(nullptr);
        dynamic_cast<Pistol*>(weapon)->setMaxAmmo(12);
    } else if (type == 1) {
        weapon = new Shotgun(nullptr);
        dynamic_cast<Shotgun*>(weapon)->setMaxAmmo(8);
    } else if (type == 2) {
        weapon = new Submachine(nullptr);
        dynamic_cast<Submachine*>(weapon)->setMaxAmmo(30);
    } else if (type == 3) {
        weapon = new Knife(nullptr);
    } else {
        weapon = new Ball(nullptr);
    }
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
}
void BattleScene::spawnRandomArmor() {
    int type = QRandomGenerator::global()->bounded(2);
    Armor *armor = nullptr;
    if (type == 0) armor = new FlamebreakerArmor(nullptr);
    else armor = new BodyArmor(nullptr);
    armor->unmount();
    qreal randomX = sceneRect().left() + 100 + QRandomGenerator::global()->bounded((int)sceneRect().width() - 200);
    armor->setPos(randomX, sceneRect().top());
    addItem(armor);
    fallingArmors.append(qMakePair(armor, QDateTime::currentMSecsSinceEpoch()));
}
void BattleScene::updateFallingWeapons() {
    const qreal gravity = 0.5;
    const qreal maxY = map->getFloorHeight();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<int> toRemove;
    QList<Weapon*> toDelete;
    for (int i = 0; i < fallingWeapons.size(); ++i) {
        Weapon *weapon = fallingWeapons[i].first;
        if (dynamic_cast<Ball*>(weapon)) {
            continue;
        }
        qint64 born = fallingWeapons[i].second;
        if (now - born > 20000 && !weapon->isMounted()) {
            removeItem(weapon);
            toDelete.append(weapon);
            toRemove.append(i);
            continue;
        }
        QPointF pos = weapon->pos();
        if (pos.y() < maxY) {
            weapon->setPos(pos.x(), std::min(maxY, pos.y() + gravity * 16));
        }
    }
    for (int i = toRemove.size() - 1; i >= 0; --i) {
        fallingWeapons.removeAt(toRemove[i]);
    }
    for (Weapon* w : toDelete) {
        delete w;
    }
}
void BattleScene::updateFallingMedicines() {
    const qreal gravity = 0.5;
    const qreal maxY = map->getFloorHeight();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<int> toRemove;
    QList<Medicine*> toDelete;
    for (int i = 0; i < fallingMedicines.size(); ++i) {
        Medicine *medicine = fallingMedicines[i].first;
        qint64 born = fallingMedicines[i].second;
        if (now - born > 10000 && !medicine->isMounted()) {
            removeItem(medicine);
            toDelete.append(medicine);
            toRemove.append(i);
            continue;
        }
        QPointF pos = medicine->pos();
        if (pos.y() < maxY) {
            medicine->setPos(pos.x(), std::min(maxY, pos.y() + gravity * 16));
        }
    }
    for (int i = toRemove.size() - 1; i >= 0; --i) {
        fallingMedicines.removeAt(toRemove[i]);
    }
    for (Medicine* m : toDelete) {
        delete m;
    }
}
void BattleScene::updateFallingArmors() {
    const qreal gravity = 0.5;
    const qreal maxY = map->getFloorHeight();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<int> toRemove;
    QList<Armor*> toDelete;
    for (int i = 0; i < fallingArmors.size(); ++i) {
        Armor *armor = fallingArmors[i].first;
        qint64 born = fallingArmors[i].second;
        if (now - born > 15000 && !armor->isMounted()) {
            removeItem(armor);
            toDelete.append(armor);
            toRemove.append(i);
            continue;
        }
        QPointF pos = armor->pos();
        if (pos.y() < maxY) {
            armor->setPos(pos.x(), std::min(maxY, pos.y() + gravity * 16));
        }
    }
    for (int i = toRemove.size() - 1; i >= 0; --i) {
        fallingArmors.removeAt(toRemove[i]);
    }
    for (Armor* a : toDelete) {
        delete a;
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
            if (character != nullptr) {
                if (!character->canAttack()) {
                    break;
                }
                if (character->getWeapon() != nullptr) {
                    Weapon *weapon = character->getWeapon();
                    if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                        character->setAttackCooldown(pistol->getFireRate());
                    } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                        character->setAttackCooldown(shotgun->getFireRate());
                    } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                        character->setAttackCooldown(submachine->getFireRate());
                    } else {
                        character->setAttackCooldown(300);
                    }
                } else {
                    character->setAttackCooldown(200);
                }
                character->startAttackCooldown();
                character->triggerAttackEffect();
                if (character->getWeapon() != nullptr) {
                    Weapon *weapon = character->getWeapon();
                    if (weapon->getWeaponName() == "Knife") {
                        QPointF charPos = character->scenePos();
                        QPointF knifePos;
                        if (character->isFacingRight()) {
                            knifePos = QPointF(charPos.x() + 33, charPos.y() - 85);
                        } else {
                            knifePos = QPointF(charPos.x() - 33, charPos.y() - 85);
                        }
                        if (hero && hero->isHitByPoint(knifePos)) {
                            hero->takeDamage(20, DamageType::Knife);
                            invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
                        } else {
                        }
                    }
                    else if (weapon->getWeaponName() == "Ball") {
                        if (weapon->getAmmo() > 0) {
                            Ball* thrownBall = new Ball(nullptr);
                            thrownBall->unmount();
                            QPointF charPos = character->scenePos();
                            thrownBall->setPos(charPos.x(), charPos.y() - 100);
                            qreal vx = character->isFacingRight() ? 15 : -15;
                            qreal vy = -25;
                            thrownBall->setVelocity(vx, vy);
                            thrownBall->setThrownMode(true);
                            thrownBall->shooter = character;
                            addItem(thrownBall);
                            removeFallingWeapon(weapon);
                            removeItem(weapon);
                            character->pickupWeapon(nullptr);
                            QTimer::singleShot(0, [weapon]() {
                                delete weapon;
                            });
                        } else {
                        }
                    } else {
                        if (weapon->getAmmo() > 0) {
                            weapon->decAmmo();
                            QPointF charPos = character->pos();
                            QPointF gunPos = weapon->scenePos();
                            qreal bulletSpeed = 22.5;
                            int bulletDamage = 20;
                            if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                                bulletSpeed = pistol->getBulletSpeed();
                                bulletDamage = pistol->getBulletDamage();
                            } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                                bulletSpeed = shotgun->getBulletSpeed();
                                bulletDamage = shotgun->getBulletDamage();
                            } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                                bulletSpeed = submachine->getBulletSpeed();
                                bulletDamage = submachine->getBulletDamage();
                            }
                            qreal vx = character->isFacingRight() ? bulletSpeed : -bulletSpeed;
                            qreal bx = gunPos.x();
                            qreal by = gunPos.y();
                            Bullet *bullet = new Bullet(bx, by, vx, bulletDamage);
                            bullet->shooter = character;
                            bullet->setZValue(100);
                            addItem(bullet);
                            if (weapon->getAmmo() <= 0) {
                                removeFallingWeapon(weapon);
                                removeItem(weapon);
                                character->pickupWeapon(nullptr);
                                QTimer::singleShot(0, [weapon]() {
                                    delete weapon;
                                });
                            }
                        } else {
                        }
                    }
                } else {
                    QPointF charPos = character->scenePos();
                    QPointF fistPos;
                    if (character->isFacingRight()) {
                        fistPos = QPointF(charPos.x() + 33, charPos.y() - 85);
                    } else {
                        fistPos = QPointF(charPos.x() - 33, charPos.y() - 85);
                    }
                    qDebug() << "[DEBUG] Fist attack position:" << fistPos;
                    if (hero && hero->isHitByPoint(fistPos)) {
                        qDebug() << "[DEBUG] Fist hit Hero! Dealing 3 damage";
                        hero->takeDamage(5, DamageType::Fist);
                        invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
                    } else {
                        qDebug() << "[DEBUG] Fist attack missed";
                    }
                }
            } else {
            }
            break;
        case Qt::Key_K:
            if (hero != nullptr) {
                if (!hero->canAttack()) {
                    break;
                }
                if (hero->getWeapon() != nullptr) {
                    Weapon *weapon = hero->getWeapon();
                    if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                        hero->setAttackCooldown(pistol->getFireRate());
                    } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                        hero->setAttackCooldown(shotgun->getFireRate());
                    } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                        hero->setAttackCooldown(submachine->getFireRate());
                    } else {
                        hero->setAttackCooldown(300);
                    }
                } else {
                    hero->setAttackCooldown(200);
                }
                hero->startAttackCooldown();
                hero->triggerAttackEffect();
                if (hero->getWeapon() != nullptr) {
                    Weapon *weapon = hero->getWeapon();
                    if (weapon->getWeaponName() == "Knife") {
                        QPointF heroPos = hero->scenePos();
                        QPointF knifePos;
                        if (hero->isFacingRight()) {
                            knifePos = QPointF(heroPos.x() + 33, heroPos.y() - 85);
                        } else {
                            knifePos = QPointF(heroPos.x() - 33, heroPos.y() - 85);
                        }
                        if (character && character->isHitByPoint(knifePos)) {
                            character->takeDamage(20, DamageType::Knife);
                            invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
                        } else {
                        }
                    } else if (weapon->getWeaponName() == "Ball") {
                        if (weapon->getAmmo() > 0) {
                            Ball* thrownBall = new Ball(nullptr);
                            thrownBall->unmount();
                            QPointF heroPos = hero->scenePos();
                            thrownBall->setPos(heroPos.x(), heroPos.y() - 100);
                            qreal vx = hero->isFacingRight() ? 15 : -15;
                            qreal vy = -25;
                            thrownBall->setVelocity(vx, vy);
                            thrownBall->setThrownMode(true);
                            thrownBall->shooter = hero;
                            addItem(thrownBall);
                            removeFallingWeapon(weapon);
                            removeItem(weapon);
                            hero->pickupWeapon(nullptr);
                            QTimer::singleShot(0, [weapon]() {
                                delete weapon;
                            });
                        } else {
                        }
                    } else {
                        if (weapon->getAmmo() > 0) {
                            weapon->decAmmo();
                            QPointF heroPos = hero->pos();
                            QPointF gunPos = weapon->scenePos();
                            qreal bulletSpeed = 22.5;
                            int bulletDamage = 20;
                            if (auto pistol = dynamic_cast<Pistol*>(weapon)) {
                                bulletSpeed = pistol->getBulletSpeed();
                                bulletDamage = pistol->getBulletDamage();
                            } else if (auto shotgun = dynamic_cast<Shotgun*>(weapon)) {
                                bulletSpeed = shotgun->getBulletSpeed();
                                bulletDamage = shotgun->getBulletDamage();
                            } else if (auto submachine = dynamic_cast<Submachine*>(weapon)) {
                                bulletSpeed = submachine->getBulletSpeed();
                                bulletDamage = submachine->getBulletDamage();
                            }
                            qreal vx = hero->isFacingRight() ? bulletSpeed : -bulletSpeed;
                            qreal bx = gunPos.x();
                            qreal by = gunPos.y();
                            Bullet *bullet = new Bullet(bx, by, vx, bulletDamage);
                            bullet->shooter = hero;
                            bullet->setZValue(100);
                            addItem(bullet);
                            if (weapon->getAmmo() <= 0) {
                                removeFallingWeapon(weapon);
                                removeItem(weapon);
                                hero->pickupWeapon(nullptr);
                                QTimer::singleShot(0, [weapon]() {
                                    delete weapon;
                                });
                            }
                        } else {
                        }
                    }
                } else {
                    QPointF heroPos = hero->scenePos();
                    QPointF fistPos;
                    if (hero->isFacingRight()) {
                        fistPos = QPointF(heroPos.x() + 33, heroPos.y() - 85);
                    } else {
                        fistPos = QPointF(heroPos.x() - 33, heroPos.y() - 85);
                    }
                    if (character && character->isHitByPoint(fistPos)) {
                        character->takeDamage(5, DamageType::Fist);
                        invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
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
    updateFallingArmors();
    frameCounter++;
    if (frameCounter >= 10) {
        frameCounter = 0;
        invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
    }
    advance();
}
void BattleScene::processMovement() {
    Scene::processMovement();
    if (character != nullptr) {
        character->applyGravity(deltaTime);
    }
    if (hero != nullptr) {
        hero->applyGravity(deltaTime);
    }
}
void BattleScene::processPicking() {
    Scene::processPicking();
    if (character && character->isPicking()) {
        auto mountable = findNearestUnmountedMountable(character->pos(), 100.);
        if (mountable != nullptr) {
            auto oldItem = pickupMountable(character, mountable);
            if (auto oldArmor = dynamic_cast<Armor *>(oldItem)) {
            }
        }
        auto medicine = findNearestMedicine(character->pos(), 100.);
        if (medicine != nullptr) {
            character->pickupMedicine(medicine);
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
        auto medicine = findNearestMedicine(hero->pos(), 100.);
        if (medicine != nullptr) {
            hero->pickupMedicine(medicine);
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
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        for (int i = 0; i < fallingArmors.size(); ++i) {
            if (fallingArmors[i].first == armor) {
                fallingArmors.removeAt(i);
                break;
            }
        }
        auto oldArmor = character->pickupArmor(armor);
        if (oldArmor) {
            QTimer::singleShot(50, [this, oldArmor]() {
                removeItem(oldArmor);
                delete oldArmor;
            });
        }
        return nullptr;
    } else if (auto weapon = dynamic_cast<Weapon *>(mountable)) {
        for (int i = 0; i < fallingWeapons.size(); ++i) {
            if (fallingWeapons[i].first == weapon) {
                fallingWeapons.removeAt(i);
                break;
            }
        }
        auto oldWeapon = character->pickupWeapon(weapon);
        if (oldWeapon) {
            qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
            qint64 shortLifeTime = currentTime - 19500;
            for (int i = 0; i < fallingWeapons.size(); ++i) {
                if (fallingWeapons[i].first == oldWeapon) {
                    fallingWeapons[i].second = shortLifeTime;
                    break;
                }
            }
            bool found = false;
            for (const auto& pair : fallingWeapons) {
                if (pair.first == oldWeapon) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                fallingWeapons.append(qMakePair(oldWeapon, shortLifeTime));
            }
        }
        return oldWeapon;
    }
    return nullptr;
}
void BattleScene::generateRandomWeapons() {
    QVector<Weapon*> weapons;
    Pistol* pistol = new Pistol(nullptr);
    pistol->setMaxAmmo(15);
    weapons.append(pistol);
    Shotgun* shotgun = new Shotgun(nullptr);
    shotgun->setMaxAmmo(8);
    weapons.append(shotgun);
    Submachine* submachine = new Submachine(nullptr);
    submachine->setMaxAmmo(45);
    weapons.append(submachine);
    weapons.append(new Knife(nullptr));
    weapons.append(new Ball(nullptr));
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(weapons.begin(), weapons.end(), g);
    std::shuffle(weapons.begin(), weapons.end(), g);
    for (int i = 0; i < weapons.size(); ++i) {
        Weapon* weapon = weapons[i];
        weapon->unmount();
        qreal randomX = sceneRect().left() + 100 + i * 200 + 
                       QRandomGenerator::global()->bounded(-50, 51);
        weapon->setPos(randomX, map->getFloorHeight());
        addItem(weapon);
    }
}
void BattleScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickPos = event->scenePos();
    qDebug() << "[DEBUG] Mouse clicked at absolute position:" << clickPos;
    Scene::mousePressEvent(event);
}
void BattleScene::debugHitBoxCorners() {
    // qDebug() << "[DEBUG] === HitBox Debug Info ===";
    if (character) {
        QRectF charHitBox = character->getHitBox();
        // qDebug() << "[DEBUG] Character HitBox:";
        // qDebug() << "  TopLeft:" << charHitBox.topLeft();
        // qDebug() << "  TopRight:" << charHitBox.topRight();
        // qDebug() << "  BottomLeft:" << charHitBox.bottomLeft();
        // qDebug() << "  BottomRight:" << charHitBox.bottomRight();
        // qDebug() << "  Center:" << charHitBox.center();
        // qDebug() << "  Size:" << charHitBox.size();
        // qDebug() << " Character Position: " << character->pos() ;
    }
    if (hero) {
        // QRectF heroHitBox = hero->getHitBox();
        // qDebug() << "[DEBUG] Hero HitBox:";
        // qDebug() << "  TopLeft:" << heroHitBox.topLeft();
        // qDebug() << "  TopRight:" << heroHitBox.topRight();
        // qDebug() << "  BottomLeft:" << heroHitBox.bottomLeft();
        // qDebug() << "  BottomRight:" << heroHitBox.bottomRight();
        // qDebug() << "  Center:" << heroHitBox.center();
        // qDebug() << "  Size:" << heroHitBox.size();
    }
    // qDebug() << "[DEBUG] === End HitBox Debug ===";
}
void BattleScene::removeFallingWeapon(Weapon* weapon) {
    // qDebug() << "[DEBUG] removeFallingWeapon: Starting to remove weapon" << weapon;
    // qDebug() << "[DEBUG] removeFallingWeapon: fallingWeapons.size() =" << fallingWeapons.size();
    bool found = false;
    for (int i = 0; i < fallingWeapons.size(); ++i) {
        if (fallingWeapons[i].first == weapon) {
            // qDebug() << "[DEBUG] removeFallingWeapon: Found weapon at index" << i;
            fallingWeapons.removeAt(i);
            found = true;
            // qDebug() << "[DEBUG] removeFallingWeapon: Weapon removed, new size =" << fallingWeapons.size();
            break;
        }
    }
    if (!found) {
        // qDebug() << "[DEBUG] removeFallingWeapon: WARNING - Weapon not found in fallingWeapons list";
        for (int i = 0; i < fallingWeapons.size(); ++i) {
            // qDebug() << "[DEBUG] removeFallingWeapon: fallingWeapons[" << i << "] =" << fallingWeapons[i].first;
        }
    }
    // qDebug() << "[DEBUG] removeFallingWeapon: Completed";
}
void BattleScene::onCharacterDied(Character* character) {
    QString winnerMessage;
    if (character == hero) {
        winnerMessage = "Player 1 Wins!";
        qDebug() << "[GAME] Hero died, Character wins!";
    } else if (character == this->character) {
        winnerMessage = "Player 2 Wins!";
        qDebug() << "[GAME] Character died, Hero wins!";
    } else {
        winnerMessage = "游戏结束";
        qDebug() << "[GAME] Unknown character died!";
    }
    QMessageBox::information(nullptr, "游戏结束", winnerMessage);
    qDebug() << "[GAME] Exiting application...";
    QApplication::quit();
}
