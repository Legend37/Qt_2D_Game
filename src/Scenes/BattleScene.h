//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QPainter>
#include <QRectF>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include "../Items/Characters/Hero.h"
#include "../Items/Weapons/Pistol.h"
#include "../Items/Weapons/Shotgun.h"
#include "../Items/Weapons/Submachine.h"
#include "../Items/Weapons/Knife.h"
#include "../Items/Weapons/Ball.h"
#include "../Items/Medicines/Bandage.h"
#include "../Items/Medicines/Medkit.h"
#include "../Items/Medicines/Adrenaline.h"
#include "../Items/Weapons/Knife.h"

class Weapon;

class BattleScene : public Scene {
Q_OBJECT

public:
    explicit BattleScene(QObject *parent);

    void processInput() override;
    void processMovement() override;
    void processPicking() override;

    // Getter methods for bullet collision detection
    Character* getCharacter() const { return character; }
    Hero* getHero() const { return hero; }
    
    // Getter method for ground height
    qreal getGroundHeight() const { return map ? map->getFloorHeight() : 520; }

protected slots:
    void update() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Mountable *findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    Medicine *findNearestMedicine(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    Mountable * pickupMountable(Character *character, Mountable *mountable);
    void generateRandomWeapons();

    void spawnRandomWeapon();
    void spawnRandomMedicine();
    void updateFallingWeapons();
    void updateFallingMedicines();
    QTimer *weaponDropTimer = nullptr;
    QTimer *medicineDropTimer = nullptr; // ҩƷ���䶨ʱ��
    QTimer *bulletDebugTimer = nullptr; // �ӵ����Զ�ʱ��
    QTimer *hitBoxDebugTimer = nullptr; // ��ײ����Զ�ʱ��
    QList<QPair<Weapon*, qint64>> fallingWeapons; // ����������ʱ��
    QList<QPair<Medicine*, qint64>> fallingMedicines; // ҩƷ������ʱ��

    Map *map;
    Character *character;
    Hero *hero;
    Armor *spareArmor;
    
    int frameCounter = 0; // ֡�����������ڿ���Ѫ�����ػ�Ƶ��

private slots:
    void debugAllBulletPositions();
    void debugHitBoxCorners();

public slots:
    void removeFallingWeapon(Weapon* weapon);
};

#endif //QT_PROGRAMMING_2024_BATTLESCENE_H
