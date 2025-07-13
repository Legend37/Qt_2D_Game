#include "Hero.h"
#include "../HeadEquipments/CapOfTheHero.h"
#include "../Armors/OldShirt.h"
#include "../LegEquipments/WellWornTrousers.h"

Hero::Hero(QGraphicsItem *parent) : Character(parent) {
    headEquipment = new CapOfTheHero(this);
    legEquipment = new WellWornTrousers(this);
    armor = new OldShirt(this);
    headEquipment->mountToParent();
    legEquipment->mountToParent();
    armor->mountToParent();
}
