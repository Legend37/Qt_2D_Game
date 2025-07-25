#ifndef QT_PROGRAMMING_2024_KNIFE_H
#define QT_PROGRAMMING_2024_KNIFE_H

#include "Weapon.h"

class Knife : public Weapon {
public:
    explicit Knife(QGraphicsItem *parent = nullptr);
    
    void mountToParent() override;
    
    // 获取和设置子弹数量（小刀不需要子弹，但为了接口一致性）
    int getMaxAmmo() const { return maxAmmo; }
    void setMaxAmmo(int ammo) { maxAmmo = ammo; setAmmo(ammo); }

private:
    int maxAmmo = 0; // 小刀不使用子弹
};

#endif
