#ifndef QT_PROGRAMMING_2024_CHARACTER_H
#define QT_PROGRAMMING_2024_CHARACTER_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QtGlobal>
#include "../HeadEquipments/HeadEquipment.h"
#include "../Armors/Armor.h"
#include "../LegEquipments/LegEquipment.h"
#include "../Weapons/Weapon.h"

// 伤害类型枚举
enum class DamageType {
    Fist,    // 拳头伤害
    Knife,   // 小刀伤害
    Bullet   // 子弹伤害
};

class Character : public QObject, public Item {
Q_OBJECT

signals:
    void characterDied(Character* character);

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
    
    // 拾取药品
    void pickupMedicine(class Medicine* medicine);

    int getHP() const { return hp; }
    void setHP(int value) { hp = value; }
    
    // 肾上腺素功能
    void activateAdrenaline();
    bool isAdrenalineActive() const { return adrenalineActive; }
    void updateAdrenalineEffect();
    
    // 获取当前护甲
    Armor* getArmor() const { return armor; }

    // 碰撞箱相关函数
    QRectF getHitBox() const;
    bool checkBulletCollision(const QPointF& bulletPos) const;
    
    // 检查给定绝对坐标是否碰到该人物
    bool isHitByPoint(const QPointF& absolutePos) const;
    
    // 攻击冷却时间相关方法
    bool canAttack() const;
    void startAttackCooldown();
    qint64 getAttackCooldown() const;
    void setAttackCooldown(qint64 cooldown);
    
    // 草地隐藏功能
    bool isOnGrassArea() const;
    
    // 冰块速度提升功能
    bool isOnIceBlock() const;
    
    // 受攻击效果
    void takeDamage(int damage, DamageType damageType = DamageType::Fist);
    void takeDamage(int damage); // 保持向后兼容性
    void updateDamageEffect();
    
    // 攻击特效
    void triggerAttackEffect();
    void updateAttackEffect();

private:
    void updateWeaponPosition();
    void updateVisibilityBasedOnGrass();

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
    static constexpr double gravity = 0.008;
    static constexpr double jumpSpeed = -2.0; // Adjusted for better jump height
    int hp = 100; // Default HP
    
    // 攻击冷却时间相关变量
    qint64 lastAttackTime = 0; // 上次攻击时间（毫秒）
    qint64 attackCooldown = 500; // 默认攻击冷却时间500毫秒
    
    // 受攻击红光效果相关变量
    int damageEffectFrames = 0; // 剩余红光帧数
    static constexpr int maxDamageEffectFrames = 10; // 红光持续10帧
    QGraphicsEffect* originalEffect = nullptr; // 保存原始效果
    
    // 攻击特效相关变量
    int attackEffectFrames = 0; // 剩余攻击特效帧数
    static constexpr int maxAttackEffectFrames = 6; // 攻击特效持续6帧
    QPointF originalPosition; // 保存原始位置
    static constexpr qreal shakeIntensity = 3.0; // 震动强度（像素）
    
    // 肾上腺素相关变量
    bool adrenalineActive = false; // 肾上腺素是否激活
    qint64 adrenalineEndTime = 0; // 肾上腺素效果结束时间（毫秒）
    static constexpr qint64 adrenalineDuration = 5000; // 肾上腺素持续时间5秒
    static constexpr double adrenalineSpeedMultiplier = 1.5; // 速度提升50%
};


#endif
