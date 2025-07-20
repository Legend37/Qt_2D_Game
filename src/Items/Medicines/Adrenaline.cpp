#include "Adrenaline.h"
#include "../Characters/Character.h"
#include <QTimer>
#include <QDebug>

Adrenaline::Adrenaline(QGraphicsItem *parent) 
    : Item(parent, ":/Items/Med/adrenaline.png", false),
      Medicine(parent, ":/Items/Med/adrenaline.png", "Adrenaline") {
}

void Adrenaline::applyEffect(Character* character) {
    if (!character) return;
    
    qDebug() << "[DEBUG] Adrenaline used - starting healing over time";
    
    // ����һ����ʱ�������������ѪЧ��
    QTimer* healingTimer = new QTimer();
    int* ticksRemaining = new int(5); // 5�룬ÿ��ظ�һ��
    
    // ���Ӷ�ʱ���źŵ�lambda����
    QObject::connect(healingTimer, &QTimer::timeout, [character, healingTimer, ticksRemaining]() {
        if (*ticksRemaining > 0 && character) {
            int currentHP = character->getHP();
            int newHP = std::min(100, currentHP + 5); // ÿ�λظ�5Ѫ������������100
            character->setHP(newHP);
            
            (*ticksRemaining)--;
            qDebug() << "[DEBUG] Adrenaline tick - HP:" << currentHP << "->" << newHP << ", ticks remaining:" << *ticksRemaining;
            
            if (*ticksRemaining <= 0) {
                qDebug() << "[DEBUG] Adrenaline effect completed";
                healingTimer->stop();
                healingTimer->deleteLater();
                delete ticksRemaining;
            }
        } else {
            // ��ɫ�����ڻ����ֹͣ��ʱ��
            healingTimer->stop();
            healingTimer->deleteLater();
            delete ticksRemaining;
        }
    });
    
    // ÿ�봥��һ��
    healingTimer->start(1000);
}
