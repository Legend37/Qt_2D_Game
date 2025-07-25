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

    // ���׹���
    virtual int getDurability() const { return durability; }
    virtual int getMaxDurability() const { return maxDurability; }
    virtual void setDurability(int value) { durability = qMax(0, qMin(value, maxDurability)); }
    virtual void takeDurabilityDamage(int damage) { setDurability(durability - damage); }
    virtual bool hasDurability() const { return maxDurability > 0; }
    
    // ����Ч��
    virtual float getFistDamageReduction() const { return 0.0f; } // ȭͷ�˺��������
    virtual float getKnifeDamageReduction() const { return 0.0f; } // С���˺��������
    virtual float getBulletDamageReduction() const { return 0.0f; } // �ӵ��˺��������
    
    // �������ͱ�ʶ
    virtual QString getArmorType() const { return "BasicArmor"; }

protected:
    int durability = 0;
    int maxDurability = 0;
};


#endif //QT_PROGRAMMING_2024_ARMOR_H
