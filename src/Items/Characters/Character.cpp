//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include <QDateTime>
#include <QGraphicsScene>
#include "Character.h"
#include "../Medicines/Medicine.h"
#include "../Maps/Battlefield.h"
#include "../Platform.h"

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
        // moveSpeed *= 1.5;
        // qDebug() << "[DEBUG] Character on ice block, 
        // speed boosted to:" << moveSpeed << "Position:" << scenePos() << "HitBox:" << getHitBox();
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
    bool onGround = false;
    
    // 检查是否在任何平台上
    if (scene()) {
        auto items = scene()->items();
        for (auto item : items) {
            if (auto battlefield = dynamic_cast<class Battlefield*>(item)) {
                // 检查是否在地面平台上
                if (battlefield->isCharacterOnGround(const_cast<Character*>(this))) {
                    onGround = true;
                    break;
                }
                // 检查是否在可跳跃平台上（仅在下降时）
                if (velocity.y() > 0 && battlefield->isCharacterOnAnyPlatform(const_cast<Character*>(this), velocity.y())) {
                    onGround = true;
                    velocity.setY(0); // 停止下降
                    break;
                }
            }
        }
        
        // 如果还没找到，检查是否在Platform对象上
        if (!onGround) {
            QList<Platform*> platforms;
            for (auto item : scene()->items()) {
                if (auto platform = dynamic_cast<Platform*>(item)) {
                    platforms.append(platform);
                }
            }
            
            qreal characterBottom = pos().y();
            for (Platform* platform : platforms) {
                QRectF rect = platform->getPlatformRect();
                // 检查角色是否正好在平台上（允许1像素误差）
                if (rect.left() < pos().x() && rect.right() > pos().x() && 
                    abs(characterBottom - rect.top()) <= 1.0) {
                    onGround = true;
                    // 只有在下降时才设置速度为0，跳跃时保持向上速度
                    if (velocity.y() >= 0) {
                        velocity.setY(0); // 确保速度为0
                    }
                    // 微调位置确保完全贴合平台
                    QPointF newPos = pos();
                    newPos.setY(rect.top());
                    setPos(newPos);
                    qDebug() << "[DEBUG] 角色已在平台上，停止下降";
                    break;
                }
            }
        }
    }
    
    if (!onGround) {
        velocity.setY(velocity.y() + gravity * deltaTime);

        // 角色下落时查找最近平台
        if (velocity.y() > 0 && scene()) {
            // 收集所有 Platform
            QList<Platform*> platforms;
            for (auto item : scene()->items()) {
                if (auto platform = dynamic_cast<Platform*>(item)) {
                    platforms.append(platform);
                }
            }
            // 使用角色绝对底部位置进行判定
            qreal characterBottom = pos().y();
            qDebug() << "[DEBUG] 角色下落中，角色位置:" << pos() << "角色底部:" << characterBottom << "速度:" << velocity.y();
            
            Platform* nearest = nullptr;
            qreal minY = std::numeric_limits<qreal>::max();
            for (Platform* platform : platforms) {
                QRectF rect = platform->getPlatformRect();
                qDebug() << "[DEBUG] 检查平台:" << rect << "角色x:" << pos().x() << "在平台x范围内:" 
                         << (rect.left() < pos().x() && rect.right() > pos().x())
                         << "平台在角色下方:" << (rect.top() > characterBottom);
                if (rect.left() < pos().x() && rect.right() > pos().x() && rect.top() > characterBottom) {
                    if (rect.top() < minY) {
                        minY = rect.top();
                        nearest = platform;
                        qDebug() << "[DEBUG] 找到候选平台，平台top:" << rect.top();
                    }
                }
            }
            if (nearest) {
                QRectF platRect = nearest->getPlatformRect();
                qreal nextBottom = characterBottom + velocity.y() * deltaTime;
                qDebug() << "[DEBUG] 选中平台:" << platRect << "当前角色底部:" << characterBottom 
                         << "下一帧角色底部:" << nextBottom << "会穿过平台:" << (nextBottom >= platRect.top());
                
                // 如果下一帧会穿过平台，立即将角色放置到平台上并停止下降
                if (nextBottom >= platRect.top()) {
                    // 落到平台
                    QPointF newPos = pos();
                    newPos.setY(platRect.top());
                    qDebug() << "[DEBUG] 角色落到平台，平台top:" << platRect.top()
                             << "角色新y:" << newPos.y()
                             << "角色底部(绝对):" << characterBottom
                             << "角色当前位置:" << pos().y();
                    setPos(newPos);
                    velocity.setY(0);
                    return;
                }
            } else {
                qDebug() << "[DEBUG] 没有找到合适的平台";
            }
        }
    }

    // 应用移动（包括水平和垂直移动）
    QPointF newPos = pos() + QPointF(velocity.x() * deltaTime, velocity.y() * deltaTime);

    // 检查是否撞击到地面（兜底机制）
    if (newPos.y() >= groundY) {
        newPos.setY(groundY);
        velocity.setY(0);
    }

    setPos(newPos);

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
                // 检查是否在地面平台上
                if (battlefield->isCharacterOnGround(const_cast<Character*>(this))) {
                    return true;
                }
                // 检查是否在任何跳跃平台上
                if (battlefield->isCharacterOnAnyPlatform(const_cast<Character*>(this), 0)) {
                    return true;
                }
            }
        }
        
        // 如果Battlefield检测没有找到，直接检查Platform对象
        QList<Platform*> platforms;
        for (auto item : scene()->items()) {
            if (auto platform = dynamic_cast<Platform*>(item)) {
                platforms.append(platform);
            }
        }
        
        qreal characterBottom = pos().y();
        for (Platform* platform : platforms) {
            QRectF rect = platform->getPlatformRect();
            // 检查角色是否正好在平台上（允许1像素误差）
            if (rect.left() < pos().x() && rect.right() > pos().x() && 
                abs(characterBottom - rect.top()) <= 1.0) {
                return true;
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
    qDebug() << "[DEBUG] Character picked up medicine:" << medicine->getMedicineName() << "Current HP:" << oldHP << "Position:" << scenePos() << "HitBox:" << getHitBox();
    
    // 立即使用药品
    medicine->applyEffect(this);
    
    int newHP = getHP();
    qDebug() << "[DEBUG] Medicine effect applied - HP changed from" << oldHP << "to" << newHP;
    
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
    // 碰撞箱：左上角(x-40, y-100)，右下角(x+40, y)，宽度80，高度100
    return QRectF(charPos.x() - 40, charPos.y() - 200, 80, 200);
}

// 判断给定的坐标是否在碰撞箱中
bool Character::checkBulletCollision(const QPointF& bulletPos) const {
    QRectF hitBox = getHitBox();
    return hitBox.contains(bulletPos);
}

// 检查给定绝对坐标是否碰到该人物
bool Character::isHitByPoint(const QPointF& absolutePos) const {
    QPointF charPos = scenePos();
    // 碰撞箱：左上角(x-40, y-100)，右下角(x+40, y)，宽度80，高度100
    QRectF hitRect(charPos.x() - 40, charPos.y() - 200, 80, 200);
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
        qDebug() << "[DEBUG] Character hidden in grass at position:" << scenePos() << "HitBox:" << getHitBox();
    } else {
        // 其他情况下显示角色和武器
        setVisible(true);
        if (weapon) {
            weapon->setVisible(true);
        }
    }
}

