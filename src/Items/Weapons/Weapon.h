#ifndef QT_PROGRAMMING_2024_WEAPON_H
#define QT_PROGRAMMING_2024_WEAPON_H

#include "../Item.h"
#include "../Mountable.h"

class Weapon : public virtual Item, public virtual Mountable {
public:
    explicit Weapon(QGraphicsItem *parent, const QString &imagePath, const QString &weaponName, bool mirrorPixmap = true);

    QString getWeaponName() const;
    void mountToParent() override;

    int getAmmo() const { return ammo; }
    void setAmmo(int n) { ammo = n; }
    void decAmmo() { if (ammo > 0) --ammo; }

    // 重写绘制方法，添加白色边框
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QString weaponName;
    int ammo = 10;
};

#endif
