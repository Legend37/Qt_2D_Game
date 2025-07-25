//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include <QDateTime>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsColorizeEffect>
#include "Character.h"
#include "../Medicines/Medicine.h"
#include "../Maps/Battlefield.h"
#include "../Platform.h"
#include "../Armors/OldShirt.h"

Character::Character(QGraphicsItem *parent) : QObject(), Item(parent, "") {
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
    
    // 更新肾上腺素效果
    updateAdrenalineEffect();
    
    // 如果肾上腺素激活，移动速度增加50%
    if (isAdrenalineActive()) {
        moveSpeed *= adrenalineSpeedMultiplier;
    }
    
    // 如果在冰块上，移动速度增加100%（翻倍）
    if (isOnIceBlock()) {
        moveSpeed *= 2.0;
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
    // 跳跃时总是输出调试信息（因为是用户主动触发的动作）
    
    bool canJump = false;
    
    // 检查是否在任何平台上（包括地面和跳跃平台）
    if (scene()) {
        auto items = scene()->items();
        for (auto item : items) {
            if (auto battlefield = dynamic_cast<class Battlefield*>(item)) {
                // 检查是否在地面平台上
                if (battlefield->isCharacterOnGround(const_cast<Character*>(this))) {
                    canJump = true;
                    break;
                }
                // 检查是否在任何跳跃平台上
                if (battlefield->isCharacterOnAnyPlatform(const_cast<Character*>(this), 0)) {
                    canJump = true;
                    break;
                }
            }
        }
        
        // 如果Battlefield没找到平台，直接检查Platform对象
        if (!canJump) {
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
                    canJump = true;
                    break;
                }
            }
        }
    }
    
    // 如果还没找到平台，检查传统的地面判定
    if (!canJump && pos().y() >= groundY) {
        canJump = true;
    }
    
    
    if (canJump) {
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
                    // 只有在下降时才设置速度为0，避免干扰跳跃
                    // 并且只在刚接触平台时设置一次
                    if (velocity.y() > 0) {
                        velocity.setY(0); // 停止下降
                        // 微调位置确保完全贴合平台
                        QPointF newPos = pos();
                        newPos.setY(rect.top());
                        setPos(newPos);
                    }
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
            
            // 控制调试输出频率 - 每秒输出一次
            static qint64 lastGravityDebugTime = 0;
            qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
            
            Platform* nearest = nullptr;
            qreal minY = std::numeric_limits<qreal>::max();
            for (Platform* platform : platforms) {
                QRectF rect = platform->getPlatformRect();
                if (rect.left() < pos().x() && rect.right() > pos().x() && rect.top() > characterBottom) {
                    if (rect.top() < minY) {
                        minY = rect.top();
                        nearest = platform;
                    }
                }
            }
            if (nearest) {
                QRectF platRect = nearest->getPlatformRect();
                qreal nextBottom = characterBottom + velocity.y() * deltaTime;
                
                // 如果下一帧会穿过平台，立即将角色放置到平台上并停止下降
                if (nextBottom >= platRect.top()) {
                    // 落到平台
                    QPointF newPos = pos();
                    newPos.setY(platRect.top());
                    setPos(newPos);
                    velocity.setY(0);
                    return;
                }
            }
        }
    }

    // 应用移动（包括水平和垂直移动）
    QPointF newPos = pos() + QPointF(velocity.x() * deltaTime, velocity.y() * deltaTime);

    // 屏幕边界检查 - 假设屏幕大小为1280x720
    const qreal screenWidth = 1280;
    const qreal screenHeight = 720;
    const qreal characterWidth = 80;  // 角色碰撞箱宽度
    const qreal characterHeight = 200; // 角色碰撞箱高度
    
    // 检查左右边界（角色中心点不能超出屏幕边界）
    if (newPos.x() - characterWidth/2 < 0) {
        newPos.setX(characterWidth/2);
        velocity.setX(0); // 停止水平移动
    } else if (newPos.x() + characterWidth/2 > screenWidth) {
        newPos.setX(screenWidth - characterWidth/2);
        velocity.setX(0); // 停止水平移动
    }
    
    // 上边界允许超出 - 移除上边界限制检查，角色可以跳出屏幕上方
    // 注释掉原有的上边界检查代码，允许角色在窗口上方自由移动
    // if (newPos.y() - characterHeight < 0) {
    //     newPos.setY(characterHeight);
    //     velocity.setY(0); // 停止向上移动
    // }
    
    // 检查是否撞击到地面（兜底机制）
    if (newPos.y() >= groundY) {
        newPos.setY(groundY);
        velocity.setY(0);
    }

    setPos(newPos);

    // 更新受攻击红光效果
    try {
        updateDamageEffect();
    } catch (const std::exception& e) {
        qDebug() << "[GRAVITY DEBUG] Exception in updateDamageEffect:" << e.what();
    } catch (...) {
        qDebug() << "[GRAVITY DEBUG] Unknown exception in updateDamageEffect";
    }
    
    // 更新攻击特效
    try {
        updateAttackEffect();
    } catch (const std::exception& e) {
        qDebug() << "[GRAVITY DEBUG] Exception in updateAttackEffect:" << e.what();
    } catch (...) {
        qDebug() << "[GRAVITY DEBUG] Unknown exception in updateAttackEffect";
    }

    static int frameCounter = 0;
    frameCounter++;
}

void Character::setGroundY(double groundY) {
    this->groundY = groundY;
}

bool Character::isOnGround() const {
    // 控制调试输出频率 - 每秒输出一次
    static qint64 lastDebugTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
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
                bool onGroundPlatform = battlefield->isCharacterOnGround(const_cast<Character*>(this));
                
                if (onGroundPlatform) {
                    return true;
                }
                // 检查是否在任何跳跃平台上
                bool onAnyPlatform = battlefield->isCharacterOnAnyPlatform(const_cast<Character*>(this), 0);
                
                if (onAnyPlatform) {
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
            bool xInRange = (rect.left() < pos().x() && rect.right() > pos().x());
            qreal yDiff = abs(characterBottom - rect.top());
            bool yInRange = (yDiff <= 1.0);
            // 检查角色是否正好在平台上（允许1像素误差）
            if (xInRange && yInRange) {
                return true;
            }
        }
    }
    
    // 如果没有找到Battlefield，使用传统的Y坐标判定
    bool result = pos().y() >= groundY;
    return result;
}

Armor *Character::pickupArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->unmount();
        oldArmor->setParentItem(parentItem());
        // 让旧盔甲掉落到新盔甲的位置
        if (newArmor != nullptr) {
            oldArmor->setPos(newArmor->pos());
        }
    }
    newArmor->setParentItem(this);
    newArmor->mountToParent();
    armor = newArmor;
    return oldArmor; // 返回旧盔甲，让BattleScene处理删除
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
    
    // 根据蹲下状态调整碰撞箱高度
    qreal height = 200; // 默认高度
    qreal yOffset = -200; // 默认Y偏移
    
    if (isCrouchDown()) {
        // 蹲下时高度变为原来的1/3
        height = 200.0 / 3.0; // 约67像素
        yOffset = -height; // Y偏移调整为新高度
    }
    
    // 碰撞箱：左上角(x-40, y+yOffset)，宽度80，高度height
    return QRectF(charPos.x() - 40, charPos.y() + yOffset, 80, height);
}

// 判断给定的坐标是否在碰撞箱中
bool Character::checkBulletCollision(const QPointF& bulletPos) const {
    QRectF hitBox = getHitBox();
    return hitBox.contains(bulletPos);
}

// 检查给定绝对坐标是否碰到该人物
bool Character::isHitByPoint(const QPointF& absolutePos) const {
    // 使用统一的碰撞箱逻辑
    QRectF hitRect = getHitBox();
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
    
    // 检查是否在冰块区域：(450-750)
    return (x >= 450 && x <= 750);
}

// 根据是否在草地上以及是否下蹲来更新可见性
void Character::updateVisibilityBasedOnGrass() {
    if (isCrouchDown() && isOnGrassArea()) {
        // 在草地上下蹲时隐藏角色和武器
        setVisible(false);
        if (weapon) {
            weapon->setVisible(false);
        }
    } else {
        // 其他情况下显示角色和武器
        setVisible(true);
        if (weapon) {
            weapon->setVisible(true);
        }
    }
}

// 受到攻击，扣除生命值并触发红光效果
void Character::takeDamage(int damage, DamageType damageType) {
    qDebug() << "[DAMAGE DEBUG] takeDamage START - damage:" << damage << "type:" << static_cast<int>(damageType) << "armor:" << armor;
    
    int originalDamage = damage;
    int actualDamage = damage;
    
    // 检查是否有护甲并计算伤害减免
    if (armor) {
        float damageReduction = 0.0f;
        
        // 根据伤害类型获取护甲的伤害减免
        switch (damageType) {
            case DamageType::Fist:
                damageReduction = armor->getFistDamageReduction();
                break;
            case DamageType::Knife:
                damageReduction = armor->getKnifeDamageReduction();
                break;
            case DamageType::Bullet:
                damageReduction = armor->getBulletDamageReduction();
                break;
        }
        
        // 计算减免后的伤害
        float reducedDamage = damage * (1.0f - damageReduction);
        actualDamage = static_cast<int>(reducedDamage);
        
        // 对于有耐久度的护甲（如BodyArmor），被护甲吸收的伤害会消耗耐久度
        if (armor->getDurability() > 0) {
            int absorbedDamage = originalDamage - actualDamage;
            // qDebug() << "[ARMOR DEBUG] Checking durability damage - Absorbed damage:" << absorbedDamage << "Current durability:" << armor->getDurability();
            
            if (absorbedDamage > 0) {
                // qDebug() << "[ARMOR DEBUG] About to call takeDurabilityDamage with:" << absorbedDamage;
                armor->takeDurabilityDamage(absorbedDamage);
                // qDebug() << "[ARMOR DEBUG] After takeDurabilityDamage, durability is:" << armor->getDurability();
                
                // 如果耐久度归零，标记需要替换护甲
                if (armor->getDurability() <= 0) {
                    // qDebug() << "[ARMOR DEBUG] Durability exhausted, starting replacement process";
                    // qDebug() << "[ARMOR DEBUG] Current armor pointer:" << armor << "Type:" << (armor ? typeid(*armor).name() : "null");
                    
                    // 保存当前护甲指针用于安全删除
                    Armor* oldArmorToDelete = armor;
                    // qDebug() << "[ARMOR DEBUG] Saved old armor pointer:" << oldArmorToDelete;
                    
                    // 立即创建并安装新的OldShirt
                    // qDebug() << "[ARMOR DEBUG] Creating new OldShirt...";
                    try {
                        armor = new OldShirt(this);
                        // qDebug() << "[ARMOR DEBUG] New OldShirt created:" << armor;
                        
                        // qDebug() << "[ARMOR DEBUG] Calling mountToParent...";
                        armor->mountToParent();
                        // qDebug() << "[ARMOR DEBUG] mountToParent completed";
                        
                        // qDebug() << "[ARMOR DEBUG] Setting visibility...";
                        armor->setVisible(true);
                        // qDebug() << "[ARMOR DEBUG] Visibility set";
                        
                        // 使用定时器安全删除旧护甲
                        // qDebug() << "[ARMOR DEBUG] Scheduling old armor deletion with timer...";
                        QTimer::singleShot(0, [this, oldArmorToDelete]() {
                            // qDebug() << "[ARMOR DEBUG] Timer callback started - deleting old armor:" << oldArmorToDelete;
                            try {
                                if (scene() && oldArmorToDelete) {
                                    // qDebug() << "[ARMOR DEBUG] Removing old armor from scene...";
                                    scene()->removeItem(oldArmorToDelete);
                                    // qDebug() << "[ARMOR DEBUG] Old armor removed from scene";
                                }
                                // qDebug() << "[ARMOR DEBUG] About to delete old armor object...";
                                delete oldArmorToDelete;
                                // qDebug() << "[ARMOR DEBUG] Old armor deleted successfully";
                            } catch (const std::exception& e) {
                                // qDebug() << "[ARMOR DEBUG] Exception during old armor deletion:" << e.what();
                            } catch (...) {
                                // qDebug() << "[ARMOR DEBUG] Unknown exception during old armor deletion";
                            }
                        });
                        // qDebug() << "[ARMOR DEBUG] Old armor deletion timer scheduled";
                        
                    } catch (const std::exception& e) {
                        // qDebug() << "[ARMOR DEBUG] Exception during armor replacement:" << e.what();
                    } catch (...) {
                        // qDebug() << "[ARMOR DEBUG] Unknown exception during armor replacement";
                    }
                }
            }
        }
    }
    
    // 扣除生命值
    //qDebug() << "[DAMAGE DEBUG] About to set HP - current HP:" << hp << "actualDamage:" << actualDamage;
    hp = qMax(0, hp - actualDamage);
    //qDebug() << "[DAMAGE DEBUG] HP set to:" << hp;
    
    // 触发红光效果
    //qDebug() << "[DAMAGE DEBUG] About to set damageEffectFrames";
    
    // 检查是否已经有红光效果在进行中
    bool wasAlreadyDamaged = (damageEffectFrames > 0);
    damageEffectFrames = maxDamageEffectFrames;
    //qDebug() << "[DAMAGE DEBUG] damageEffectFrames set to:" << damageEffectFrames << "wasAlreadyDamaged:" << wasAlreadyDamaged;
    
    // 只有在没有红光效果时才创建新的红光效果
    if (!wasAlreadyDamaged) {
        // qDebug() << "[DAMAGE DEBUG] Creating new red effect (first damage)";
        QGraphicsColorizeEffect* redEffect = new QGraphicsColorizeEffect();
        redEffect->setColor(Qt::red);
        redEffect->setStrength(0.8); // 80%的红色强度
        // qDebug() << "[DAMAGE DEBUG] Red effect created";
        
        // 保存原始效果并应用红光效果（只在第一次受伤时保存）
        // qDebug() << "[DAMAGE DEBUG] About to save original effect and apply red effect";
        originalEffect = graphicsEffect();
        // qDebug() << "[DAMAGE DEBUG] Original effect saved:" << originalEffect;
        setGraphicsEffect(redEffect);
        // qDebug() << "[DAMAGE DEBUG] Red effect applied to character";
        
        // 如果有武器，也应用红光效果
        if (weapon) {
            // qDebug() << "[DAMAGE DEBUG] About to apply red effect to weapon";
            QGraphicsColorizeEffect* weaponRedEffect = new QGraphicsColorizeEffect();
            weaponRedEffect->setColor(Qt::red);
            weaponRedEffect->setStrength(0.8);
            weapon->setGraphicsEffect(weaponRedEffect);
            // qDebug() << "[DAMAGE DEBUG] Red effect applied to weapon";
        } else {
            // qDebug() << "[DAMAGE DEBUG] No weapon to apply red effect to";
        }
    } else {
        // qDebug() << "[DAMAGE DEBUG] Red effect already active, just resetting frames";
    }
    
    // qDebug() << "[DAMAGE] Character took" << actualDamage << "damage, HP:" << hp << "Red effect frames:" << damageEffectFrames;
    // qDebug() << "[DAMAGE DEBUG] takeDamage END - armor after damage:" << armor << "HP:" << hp;
    
    // 检查角色是否死亡
    if (hp <= 0) {
        // qDebug() << "[DEATH] Character died, emitting characterDied signal";
        emit characterDied(this);
    }
}

// 向后兼容的takeDamage函数
void Character::takeDamage(int damage) {
    takeDamage(damage, DamageType::Fist); // 默认为拳头伤害
}

// 更新受攻击效果，需要在每帧调用
void Character::updateDamageEffect() {
    if (damageEffectFrames > 0) {
        // qDebug() << "[DAMAGE EFFECT DEBUG] Updating damage effect, frames remaining:" << damageEffectFrames;
        damageEffectFrames--;
        
        // 如果红光效果结束，恢复原始效果
        if (damageEffectFrames <= 0) {
            // qDebug() << "[DAMAGE EFFECT DEBUG] Damage effect ending, restoring original effect";
            try {
                // 安全地恢复原始效果
                // qDebug() << "[DAMAGE EFFECT DEBUG] Current originalEffect pointer:" << originalEffect;
                
                // 首先移除当前的红光效果
                setGraphicsEffect(nullptr);
                // qDebug() << "[DAMAGE EFFECT DEBUG] Current effect cleared";
                
                // 然后设置原始效果（如果存在的话）
                if (originalEffect != nullptr) {
                    setGraphicsEffect(originalEffect);
                    // qDebug() << "[DAMAGE EFFECT DEBUG] Original effect restored";
                } else {
                    // qDebug() << "[DAMAGE EFFECT DEBUG] No original effect to restore";
                }
                originalEffect = nullptr;
                // qDebug() << "[DAMAGE EFFECT DEBUG] Character effect restoration completed";
                
                // 移除武器的红光效果
                if (weapon) {
                    // qDebug() << "[DAMAGE EFFECT DEBUG] Removing weapon red effect";
                    weapon->setGraphicsEffect(nullptr);
                    // qDebug() << "[DAMAGE EFFECT DEBUG] Weapon effect removed";
                }
                
                // qDebug() << "[DAMAGE] Red effect ended, HP:" << hp;
            } catch (const std::exception& e) {
                // qDebug() << "[DAMAGE EFFECT DEBUG] Exception during effect restoration:" << e.what();
            } catch (...) {
                qDebug() << "[DAMAGE EFFECT DEBUG] Unknown exception during effect restoration";
            }
        }
    }
}

// 触发攻击特效
void Character::triggerAttackEffect() {
    attackEffectFrames = maxAttackEffectFrames;
    originalPosition = pos(); // 保存当前位置
    // qDebug() << "[ATTACK EFFECT] Attack effect triggered, frames:" << attackEffectFrames;
}

// 更新攻击特效，需要在每帧调用
void Character::updateAttackEffect() {
    if (attackEffectFrames > 0) {
        // 只在攻击特效激活时才应用震动，避免干扰正常移动
        if (attackEffectFrames == maxAttackEffectFrames) {
            // 第一帧，保存当前位置作为震动基准
            originalPosition = pos();
        }
        
        attackEffectFrames--;
        
        // 创建左右震动效果
        qreal shakeOffset = 0;
        if (attackEffectFrames % 2 == 0) {
            shakeOffset = shakeIntensity; // 向右偏移
        } else {
            shakeOffset = -shakeIntensity; // 向左偏移
        }
        
        // 应用震动偏移（基于当前的originalPosition）
        QPointF shakePos = originalPosition;
        shakePos.setX(originalPosition.x() + shakeOffset);
        setPos(shakePos);
        
        // 如果攻击特效结束，恢复原始位置
        if (attackEffectFrames <= 0) {
            setPos(originalPosition);
            // qDebug() << "[ATTACK EFFECT] Attack effect ended, position restored";
        }
    }
}

// 肾上腺素功能实现
void Character::activateAdrenaline() {
    adrenalineActive = true;
    adrenalineEndTime = QDateTime::currentMSecsSinceEpoch() + adrenalineDuration;
    // qDebug() << "[ADRENALINE] Adrenaline activated for" << adrenalineDuration << "ms";
}

void Character::updateAdrenalineEffect() {
    if (adrenalineActive) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime >= adrenalineEndTime) {
            adrenalineActive = false;
            // qDebug() << "[ADRENALINE] Adrenaline effect ended";
        }
    }
}

