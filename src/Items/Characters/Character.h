#ifndef QT_PROGRAMMING_2024_CHARACTER_H
#define QT_PROGRAMMING_2024_CHARACTER_H

#include <QGraphicsEllipseItem>
#include <QtGlobal>
#include "../HeadEquipments/HeadEquipment.h"
#include "../Armors/Armor.h"
#include "../LegEquipments/LegEquipment.h"
#include "../Weapons/Weapon.h"

class Character : public Item {
public:
    Weapon* getWeapon() const { return weapon; }
    bool isFacingRight() const { return facingRight; }
    explicit Character(QGraphicsItem *parent);

    [[nodiscard]] bool isLeftDown() const;

    void setLeftDown(bool leftDown);

    [[nodiscard]] bool isRightDown() const;

    void setRightDown(bool rightDown);

    [[nodiscard]] bool isPickDown() const;

    void setPickDown(bool pickDown);

    [[nodiscard]] bool isCrouchDown() const;
    void setCrouchDown(bool crouchDown);

    [[nodiscard]] const QPointF &getVelocity() const;

    [[nodiscard]] bool isPicking() const;

    void setVelocity(const QPointF &velocity);

    void processInput();

    void jump();

    void applyGravity(double deltaTime);

    void setGroundY(double groundY);

    [[nodiscard]] bool isOnGround() const;

    Armor* pickupArmor(Armor* newArmor);

    Weapon* pickupWeapon(Weapon* newWeapon);

    int getHP() const { return hp; }
    void setHP(int value) { hp = value; }

    // ��ײ����غ���
    QRectF getHitBox() const;
    bool checkBulletCollision(const QPointF& bulletPos) const;
    
    // ���������������Ƿ�����������
    bool isHitByPoint(const QPointF& absolutePos) const;
    
    // ������ȴʱ����ط���
    bool canAttack() const;
    void startAttackCooldown();
    qint64 getAttackCooldown() const;
    void setAttackCooldown(qint64 cooldown);

private:
    void updateWeaponPosition();

protected:
    HeadEquipment *headEquipment{};
    LegEquipment *legEquipment{};
    Armor *armor{};
    Weapon *weapon{};
    QPointF velocity{};
//    QGraphicsEllipseItem *ellipseItem; // for debugging
private:
    bool leftDown{}, rightDown{}, pickDown{}, crouchDown{};
    bool lastPickDown{};
    bool picking{};
    bool facingRight{true}; //record the facing direction of the character
    // If the character is facing right, the x coordinate of the pixmap will be negative
    double groundY{};
    static constexpr double gravity = 0.02;
    static constexpr double jumpSpeed = -3.0;
    int hp = 100; // Default HP
    
    // ������ȴʱ����ر���
    qint64 lastAttackTime = 0; // �ϴι���ʱ�䣨���룩
    qint64 attackCooldown = 500; // Ĭ�Ϲ�����ȴʱ��500����
};


#endif
