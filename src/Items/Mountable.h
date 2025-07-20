//
// Created by gerw on 8/21/24.
//


#ifndef QT_PROGRAMMING_2024_MOUNTABLE_H
#define QT_PROGRAMMING_2024_MOUNTABLE_H

#include "Item.h"
#include <QString>

class Mountable : public virtual Item {
public:
    explicit Mountable(QGraphicsItem* parent = nullptr, const QString& pixmapPath = "")
        : Item(parent, pixmapPath) {}

    virtual void mountToParent();
    virtual void unmount();
    [[nodiscard]] bool isMounted() const;

private:
    bool mounted{};
};

#endif //QT_PROGRAMMING_2024_MOUNTABLE_H
