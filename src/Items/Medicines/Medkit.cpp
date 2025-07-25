#include "Medkit.h"
#include "../Characters/Character.h"
#include <QDebug>

Medkit::Medkit(QGraphicsItem *parent) 
    : Item(parent, ":/Items/Med/medkit.png", false),
      Medicine(parent, ":/Items/Med/medkit.png", "Medkit") {
}

void Medkit::applyEffect(Character* character) {
    if (!character) return;
    
    int currentHP = character->getHP();
    character->setHP(100); 
    
    qDebug() << "[DEBUG] Medkit used - HP restored from" << currentHP << "to 100 (full health)";
}
