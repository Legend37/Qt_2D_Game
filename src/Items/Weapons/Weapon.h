#ifndef QT_PROGRAMMING_2024_WEAPON_H
#define QT_PROGRAMMING_2024_WEAPON_H

#include "../Item.h"
#include "../Mountable.h"

class Weapon : public Item, public Mountable {
public:
    explicit Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName, bool mirrorPixmap = true);
    
    QString getWeaponName() const;
    
    void mountToParent() override;
    
protected:
    QString weaponName;
};

#endif
