//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_ARMOR_H
#define QT_PROGRAMMING_2024_ARMOR_H

#include "../Item.h"
#include "../Mountable.h"

class Armor : public virtual Item, public virtual Mountable {
public:
    explicit Armor(QGraphicsItem *parent, const QString &pixmapPath);

    void mountToParent() override;

    void unmount() override;
};


#endif //QT_PROGRAMMING_2024_ARMOR_H
