//
// Created by gerw on 8/20/24.
//

#include <memory>
#include "Link.h"
#include "../HeadEquipments/CapOfTheCharacter.h"
#include "../Armors/OldShirt.h"
#include "../LegEquipments/CharacterTrousers.h"

Link::Link(QGraphicsItem *parent) : Character(parent) {
    headEquipment = new CapOfTheCharacter(this);
    legEquipment = new CharacterTrousers(this);
    armor = new OldShirt(this);
    headEquipment->mountToParent();
    legEquipment->mountToParent();
    armor->mountToParent();
}
