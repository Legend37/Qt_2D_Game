//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_HEADEQUIPMENT_H
#define QT_PROGRAMMING_2024_HEADEQUIPMENT_H


#include "../Item.h"
#include "../Mountable.h"

class HeadEquipment: public virtual Item, public virtual Mountable {
public:
    explicit HeadEquipment(QGraphicsItem *parent, const QString &pixmapPath);

    void mountToParent() override;
};


#endif //QT_PROGRAMMING_2024_HEADEQUIPMENT_H
