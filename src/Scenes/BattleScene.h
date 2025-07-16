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

protected slots:
    void update() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Mountable *findNearestUnmountedMountable(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    static Mountable * pickupMountable(Character *character, Mountable *mountable);
    void generateRandomWeapons();

    void spawnRandomWeapon();
    void updateFallingWeapons();
    QTimer *weaponDropTimer = nullptr;
    QTimer *bulletDebugTimer = nullptr; // 子弹调试定时器
    QList<QPair<Weapon*, qint64>> fallingWeapons; // 武器和生成时间

    Map *map;
    Character *character;
    Hero *hero;
    Armor *spareArmor;

private slots:
    void debugAllBulletPositions();
};

#endif //QT_PROGRAMMING_2024_BATTLESCENE_H
