//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include "Character.h"

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
    const auto moveSpeed = 0.3;
    
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
    // Check if the character is on the ground
    // If the character's position is below the ground level, reset its position to the ground
    double newY = pos().y() + velocity.y() * deltaTime;
    if (newY >= groundY) {
        setPos(pos().x(), groundY);
        velocity.setY(0);
    }
}

void Character::setGroundY(double groundY) {
    this->groundY = groundY;
}

bool Character::isOnGround() const {
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
        oldWeapon->setPos(newWeapon->pos());
        oldWeapon->setParentItem(parentItem());
    }
    newWeapon->setParentItem(this);
    newWeapon->mountToParent();
    weapon = newWeapon;
    updateWeaponPosition(); // 确保武器位置正确
    return oldWeapon;
}

void Character::updateWeaponPosition() {
    if (weapon != nullptr) {
        // 根据角色朝向调整武器位置
        if (facingRight) {
            // 面向左边时（翻转状态），武器应该在左手
            weapon->setPos(-20, -5);
            weapon->setRotation(15); // 镜像旋转
        } else {
            // 面向右边时，武器在右手
            weapon->setPos(20, -5);
            weapon->setRotation(-15);
        }
    }
}

