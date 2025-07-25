//
// Created for Qt Programming 2025
//

#include "BodyArmor.h"

BodyArmor::BodyArmor(QGraphicsItem *parent)
    : Item(parent, ":/Items/Armors/bodyarmor.png", false),
      Armor(parent, ":/Items/Armors/bodyarmor.png") {
    // BodyArmor特殊初始化：设置耐久度
    maxDurability = 100;
    durability = 100;
}
