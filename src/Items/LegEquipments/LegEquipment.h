//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_LEGEQUIPMENT_H
#define QT_PROGRAMMING_2024_LEGEQUIPMENT_H


#include "../Item.h"
#include "../Mountable.h"

class LegEquipment: public virtual Item, public virtual Mountable {
public:
    explicit LegEquipment(QGraphicsItem *parent, const QString &pixmapPath);

    void mountToParent() override;
};


#endif //QT_PROGRAMMING_2024_LEGEQUIPMENT_H
