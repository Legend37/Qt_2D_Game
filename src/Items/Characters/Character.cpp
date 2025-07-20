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
    
    // ����ڱ����ϣ��ƶ��ٶ�����50%
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
        updateWeaponPosition(); // �¶�ʱҲ��������λ��
        updateVisibilityBasedOnGrass(); // ����Ƿ��ڲݵ��ϲ����¿ɼ���
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
        updateWeaponPosition(); // ��������λ��
        updateVisibilityBasedOnGrass(); // �ָ��ɼ��ԣ���������¶�״̬��
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
    
    // Ӧ���ƶ�
    QPointF newPos = pos() + QPointF(0, velocity.y() * deltaTime);
    
    // ����Ƿ�ײ����ƽ̨�����
    if (newPos.y() >= groundY) {
        newPos.setY(groundY);
        velocity.setY(0);
    }
    
    setPos(newPos);
    
    // ÿ�����һ��(200,500)�Ƿ�����ײ���ڣ�����60fps��
    static int frameCounter = 0;
    frameCounter++;
}

void Character::setGroundY(double groundY) {
    this->groundY = groundY;
}

bool Character::isOnGround() const {
    // �ȼ�������Y�����ж������׻��ƣ�
    if (pos().y() < groundY) {
        return false;
    }
    
    // ���Ի�ȡBattleScene��ʹ��ƽ̨�ж�
    if (scene()) {
        // ��鳡�����Ƿ���Battlefield
        auto items = scene()->items();
        for (auto item : items) {
            if (auto battlefield = dynamic_cast<class Battlefield*>(item)) {
                return battlefield->isCharacterOnGround(const_cast<Character*>(this));
            }
        }
    }
    
    // ���û���ҵ�Battlefield��ʹ�ô�ͳ��Y�����ж�
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
    updateWeaponPosition(); // ȷ������λ����ȷ
    return oldWeapon;
}

void Character::pickupMedicine(Medicine* medicine) {
    if (!medicine) return;
    
    int oldHP = getHP();
    // qDebug() << "[DEBUG] Character picked up medicine:" << medicine->getMedicineName() << "Current HP:" << oldHP;
    
    // ����ʹ��ҩƷ
    medicine->applyEffect(this);
    
    int newHP = getHP();
    // qDebug() << "[DEBUG] Medicine effect applied - HP changed from" << oldHP << "to" << newHP;
    
    // ǿ�Ƹ��³�����ʾ
    if (scene()) {
        scene()->update();
    }
}

void Character::updateWeaponPosition() {
    if (weapon != nullptr) {
        // ����ʼ���ڽ�ɫ�����Ϸ�������Ҫ���ݳ������λ��
        weapon->setPos(-60, -120);
        weapon->setRotation(0);
        weapon->setZValue(2); // ȷ�������ϲ�
    }
}

QRectF Character::getHitBox() const {
    QPointF charPos = scenePos();
    return QRectF(charPos.x(), charPos.y() - 200, 100, 200);
}

// �жϸ����������Ƿ�����ײ����
bool Character::checkBulletCollision(const QPointF& bulletPos) const {
    QRectF hitBox = getHitBox();
    return hitBox.contains(bulletPos);
}

// ���������������Ƿ�����������
bool Character::isHitByPoint(const QPointF& absolutePos) const {
    QPointF charPos = scenePos();
    // ������posΪ���½ǣ�pos.x+100, pos.y-200Ϊ���Ͻǵľ���
    QRectF hitRect(charPos.x(), charPos.y() - 200, 100, 200);
    return hitRect.contains(absolutePos);
}

// ����Ƿ���Թ�������ȴʱ���Ƿ������
bool Character::canAttack() const {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    return (currentTime - lastAttackTime) >= attackCooldown;
}

// ��ʼ������ȴ��ʱ
void Character::startAttackCooldown() {
    lastAttackTime = QDateTime::currentMSecsSinceEpoch();
}

// ��ȡ������ȴʱ�䣨���룩
qint64 Character::getAttackCooldown() const {
    return attackCooldown;
}

// ���ù�����ȴʱ�䣨���룩
void Character::setAttackCooldown(qint64 cooldown) {
    attackCooldown = cooldown;
}

// ����ɫ�Ƿ��ڲݵ�����
bool Character::isOnGrassArea() const {
    QPointF charPos = scenePos();
    qreal x = charPos.x();
    
    // ����Ƿ��ڵ�����
    if (!isOnGround()) {
        return false;
    }
    
    // ����Ƿ��ڲݵ�����(300-450) �� (900-1050)
    return (x >= 300 && x <= 450) || (x >= 900 && x <= 1050);
}

// ����ɫ�Ƿ��ڱ�������
bool Character::isOnIceBlock() const {
    QPointF charPos = scenePos();
    qreal x = charPos.x();
    
    // ����Ƿ��ڵ�����
    if (!isOnGround()) {
        return false;
    }
    
    // ����Ƿ��ڱ�������(576-710)
    return (x >= 576 && x <= 710);
}

// �����Ƿ��ڲݵ����Լ��Ƿ��¶������¿ɼ���
void Character::updateVisibilityBasedOnGrass() {
    if (isCrouchDown() && isOnGrassArea()) {
        // �ڲݵ����¶�ʱ���ؽ�ɫ������
        setVisible(false);
        if (weapon) {
            weapon->setVisible(false);
        }
        qDebug() << "[DEBUG] Character hidden in grass at position:" << scenePos();
    } else {
        // �����������ʾ��ɫ������
        setVisible(true);
        if (weapon) {
            weapon->setVisible(true);
        }
    }
}

