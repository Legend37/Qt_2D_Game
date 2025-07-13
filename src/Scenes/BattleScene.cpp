//
// Created by gerw on 8/20/24.
//

#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>
#include "BattleScene.h"
#include "../Items/Characters/Link.h"
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
    
    // �ڵ��������������
    generateRandomWeapons();
}

void BattleScene::processInput() {
    Scene::processInput();
    if (character != nullptr) {
        character->processInput();
    }
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_A:
            if (character != nullptr) {
                character->setLeftDown(true);
            }
            break;
        case Qt::Key_D:
            if (character != nullptr) {
                character->setRightDown(true);
            }
            break;
        case Qt::Key_J:
            if (character != nullptr) {
                character->setPickDown(true);
            }
            break;
        case Qt::Key_W:
            if (character != nullptr) {
                character->jump();
            }
            break;
        case Qt::Key_S:
            if (character != nullptr) {
                character->setCrouchDown(true);
            }
            break;
        default:
            Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_A:
            if (character != nullptr) {
                character->setLeftDown(false);
            }
            break;
        case Qt::Key_D:
            if (character != nullptr) {
                character->setRightDown(false);
            }
            break;
        case Qt::Key_J:
            if (character != nullptr) {
                character->setPickDown(false);
            }
            break;
        case Qt::Key_S:
            if (character != nullptr) {
                character->setCrouchDown(false);
            }
            break;
        default:
            Scene::keyReleaseEvent(event);
    }
}

void BattleScene::update() {
    Scene::update();
}

void BattleScene::processMovement() {
    Scene::processMovement();
    if (character != nullptr) {
        character->applyGravity(deltaTime);
        character->setPos(character->pos() + character->getVelocity() * (double) deltaTime);
    }
}

void BattleScene::processPicking() {
    Scene::processPicking();
    if (character->isPicking()) {
        auto mountable = findNearestUnmountedMountable(character->pos(), 100.);
        if (mountable != nullptr) {
            auto oldItem = pickupMountable(character, mountable);
            if (auto oldArmor = dynamic_cast<Armor *>(oldItem)) {
                spareArmor = oldArmor;
            }
            // ��������������������Զ����䵽����
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
    // ֧�ֻ��׺�����
    if (auto armor = dynamic_cast<Armor *>(mountable)) {
        return character->pickupArmor(armor);
    } else if (auto weapon = dynamic_cast<Weapon *>(mountable)) {
        return character->pickupWeapon(weapon);
    }
    return nullptr;
}

void BattleScene::generateRandomWeapons() {
    // ��������3���������ͣ�ÿ��ֻ����һ��
    QVector<Weapon*> weapons;
    
    // ����ÿ������
    weapons.append(new Pistol(nullptr));
    weapons.append(new Shotgun(nullptr));
    weapons.append(new Submachine(nullptr));
    
    // �����������˳��
    std::random_shuffle(weapons.begin(), weapons.end());
    
    // �ڵ������λ�÷�������
    for (int i = 0; i < weapons.size(); ++i) {
        Weapon* weapon = weapons[i];
        weapon->unmount();
        
        // ���λ���ڵ����ϣ�ȷ������֮����һ������
        qreal randomX = sceneRect().left() + 100 + i * 200 + 
                       QRandomGenerator::global()->bounded(-50, 51);
        
        weapon->setPos(randomX, map->getFloorHeight());
        addItem(weapon);
    }
}
