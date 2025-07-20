//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include <QDateTime>
#include <QGraphicsScene>
#include "Character.h"
#include "../Medicines/Medicine.h"
#include "../Maps/Battlefield.h"

Character::Character(QGraphicsItem *parent) : Item(parent, "") {
//    ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
//    // Optionally, set some properties of the ellipse
//    ellipseItem->setBrush(Qt::green);          // Fill color
//    ellipseItem->setZValue(1);
}

bool Character::isLeftDown() const {
    return leftDown;
}

void Character::setLeftDown(bool leftDown) {
    Character::leftDown = leftDown;
}

bool Character::isRightDown() const {
    return rightDown;
}

void Character::setRightDown(bool rightDown) {
    Character::rightDown = rightDown;
}

bool Character::isPickDown() const {
    return pickDown;
}

void Character::setPickDown(bool pickDown) {
    Character::pickDown = pickDown;
}

const QPointF &Character::getVelocity() const {
    return velocity;
}

void Character::setVelocity(const QPointF &velocity) {
    Character::velocity = velocity;
}

bool Character::isCrouchDown() const {
    return crouchDown;
}

void Character::setCrouchDown(bool crouchDown) {
    Character::crouchDown = crouchDown;
}

void Character::processInput() {
    auto newVelocity = QPointF(0, velocity.y()); // Reset horizontal velocity
    auto moveSpeed = 0.5;
    
    // 如果在冰块上，移动速度增加50%
    if (isOnIceBlock()) {
        moveSpeed *= 1.5;
        // qDebug() << "[DEBUG] Character on ice block, speed boosted to:" << moveSpeed;
    }
    
    if (isCrouchDown()) {
        if (isLeftDown()) {
            facingRight = true;
        } else if (isRightDown()) {
            facingRight = false;
        }
        QTransform transform;
        if (facingRight) {
            transform.scale(-1, 0.5).translate(0, 20); 
        } else {
            transform.scale(1, 0.5).translate(0, 20);
        }
        setTransform(transform);
        updateWeaponPosition(); // 下蹲时也更新武器位置
        updateVisibilityBasedOnGrass(); // 检查是否在草地上并更新可见性
    } else {
        if (isLeftDown()) {
            newVelocity.setX(newVelocity.x() - moveSpeed);
            facingRight = false; 
        }
        if (isRightDown()) {
            newVelocity.setX(newVelocity.x() + moveSpeed);
            facingRight = true;
        }
        
        QTransform transform;
        if (facingRight) {
            transform.scale(-1, 1); 
        } else {
            transform.scale(1, 1);  
        }
        setTransform(transform);
        updateWeaponPosition(); // 更新武器位置
        updateVisibilityBasedOnGrass(); // 恢复可见性（如果不在下蹲状态）
    }
    setVelocity(newVelocity);

    if (!lastPickDown && pickDown) {
        picking = true;
    } else {
        picking = false;
    }
    lastPickDown = pickDown;
}

bool Character::isPicking() const {
    return picking;
}

void Character::jump() {
    if (isOnGround()) {
        velocity.setY(jumpSpeed);
    }
}

void Character::applyGravity(double deltaTime) {
    if (!isOnGround()) {
        velocity.setY(velocity.y() + gravity * deltaTime);
    }
    
    // 应用移动
    QPointF newPos = pos() + QPointF(0, velocity.y() * deltaTime);
    
    // 检查是否撞击到平台或地面
    if (newPos.y() >= groundY) {
        newPos.setY(groundY);
        velocity.setY(0);
    }
    
    setPos(newPos);
    
    // 每秒输出一次(200,500)是否在碰撞箱内（假设60fps）
    static int frameCounter = 0;
    frameCounter++;
}

void Character::setGroundY(double groundY) {
    this->groundY = groundY;
}

bool Character::isOnGround() const {
    // 先检查基本的Y坐标判定（兜底机制）
    if (pos().y() < groundY) {
        return false;
    }
    
    // 尝试获取BattleScene，使用平台判定
    if (scene()) {
        // 检查场景中是否有Battlefield
        auto items = scene()->items();
        for (auto item : items) {
            if (auto battlefield = dynamic_cast<class Battlefield*>(item)) {
                return battlefield->isCharacterOnGround(const_cast<Character*>(this));
            }
        }
    }
    
    // 如果没有找到Battlefield，使用传统的Y坐标判定
    return pos().y() >= groundY;
}

Armor *Character::pickupArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->unmount();
        oldArmor->setPos(newArmor->pos());
        oldArmor->setParentItem(parentItem());
    }
    newArmor->setParentItem(this);
    newArmor->mountToParent();
    armor = newArmor;
    return oldArmor;
}

Weapon *Character::pickupWeapon(Weapon *newWeapon) {
    auto oldWeapon = weapon;
    if (oldWeapon != nullptr) {
        oldWeapon->unmount();
        // Only set position if newWeapon is not nullptr
        if (newWeapon != nullptr) {
            oldWeapon->setPos(newWeapon->pos());
        }
        oldWeapon->setParentItem(parentItem());
    }
    
    // Only mount newWeapon if it's not nullptr
    if (newWeapon != nullptr) {
        newWeapon->setParentItem(this);
        newWeapon->mountToParent();
    }
    
    weapon = newWeapon;
    updateWeaponPosition(); // 确保武器位置正确
    return oldWeapon;
}

void Character::pickupMedicine(Medicine* medicine) {
    if (!medicine) return;
    
    int oldHP = getHP();
    // qDebug() << "[DEBUG] Character picked up medicine:" << medicine->getMedicineName() << "Current HP:" << oldHP;
    
    // 立即使用药品
    medicine->applyEffect(this);
    
    int newHP = getHP();
    // qDebug() << "[DEBUG] Medicine effect applied - HP changed from" << oldHP << "to" << newHP;
    
    // 强制更新场景显示
    if (scene()) {
        scene()->update();
    }
}

void Character::updateWeaponPosition() {
    if (weapon != nullptr) {
        // 武器始终在角色中心上方，不需要根据朝向调整位置
        weapon->setPos(-60, -120);
        weapon->setRotation(0);
        weapon->setZValue(2); // 确保在最上层
    }
}

QRectF Character::getHitBox() const {
    QPointF charPos = scenePos();
    return QRectF(charPos.x(), charPos.y() - 200, 100, 200);
}

// 判断给定的坐标是否在碰撞箱中
bool Character::checkBulletCollision(const QPointF& bulletPos) const {
    QRectF hitBox = getHitBox();
    return hitBox.contains(bulletPos);
}

// 检查给定绝对坐标是否碰到该人物
bool Character::isHitByPoint(const QPointF& absolutePos) const {
    QPointF charPos = scenePos();
    // 以人物pos为左下角，pos.x+100, pos.y-200为右上角的矩形
    QRectF hitRect(charPos.x(), charPos.y() - 200, 100, 200);
    return hitRect.contains(absolutePos);
}

// 检查是否可以攻击（冷却时间是否结束）
bool Character::canAttack() const {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    return (currentTime - lastAttackTime) >= attackCooldown;
}

// 开始攻击冷却计时
void Character::startAttackCooldown() {
    lastAttackTime = QDateTime::currentMSecsSinceEpoch();
}

// 获取攻击冷却时间（毫秒）
qint64 Character::getAttackCooldown() const {
    return attackCooldown;
}

// 设置攻击冷却时间（毫秒）
void Character::setAttackCooldown(qint64 cooldown) {
    attackCooldown = cooldown;
}

// 检查角色是否在草地区域
bool Character::isOnGrassArea() const {
    QPointF charPos = scenePos();
    qreal x = charPos.x();
    
    // 检查是否在地面上
    if (!isOnGround()) {
        return false;
    }
    
    // 检查是否在草地区域：(300-450) 或 (900-1050)
    return (x >= 300 && x <= 450) || (x >= 900 && x <= 1050);
}

// 检查角色是否在冰块区域
bool Character::isOnIceBlock() const {
    QPointF charPos = scenePos();
    qreal x = charPos.x();
    
    // 检查是否在地面上
    if (!isOnGround()) {
        return false;
    }
    
    // 检查是否在冰块区域：(576-710)
    return (x >= 576 && x <= 710);
}

// 根据是否在草地上以及是否下蹲来更新可见性
void Character::updateVisibilityBasedOnGrass() {
    if (isCrouchDown() && isOnGrassArea()) {
        // 在草地上下蹲时隐藏角色和武器
        setVisible(false);
        if (weapon) {
            weapon->setVisible(false);
        }
        qDebug() << "[DEBUG] Character hidden in grass at position:" << scenePos();
    } else {
        // 其他情况下显示角色和武器
        setVisible(true);
        if (weapon) {
            weapon->setVisible(true);
        }
    }
}

