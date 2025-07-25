#include "Bandage.h"
#include "../Characters/Character.h"
#include <QDebug>

Bandage::Bandage(QGraphicsItem *parent) 
    : Item(parent, ":/Items/Med/bandage.png", false),
      Medicine(parent, ":/Items/Med/bandage.png", "Bandage") {
}

void Bandage::applyEffect(Character* character) {
    if (!character) return;
    
    int currentHP = character->getHP();
    int newHP = std::min(100, currentHP + 20); // 恢复20血量，但不超过100
    character->setHP(newHP);
    
    qDebug() << "[DEBUG] Bandage used - HP restored from" << currentHP << "to" << newHP;
}
