//
// Created for Qt Programming 2025
//

#include "BodyArmor.h"

BodyArmor::BodyArmor(QGraphicsItem *parent)
    : Item(parent, ":/Items/Armors/bodyarmor.png", false),
      Armor(parent, ":/Items/Armors/bodyarmor.png") {
    // BodyArmor�����ʼ���������;ö�
    maxDurability = 100;
    durability = 100;
}
