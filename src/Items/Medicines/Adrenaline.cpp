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
    
    // 创建一个定时器来处理持续回血效果
    QTimer* healingTimer = new QTimer();
    int* ticksRemaining = new int(5); // 5秒，每秒回复一次
    
    // 连接定时器信号到lambda函数
    QObject::connect(healingTimer, &QTimer::timeout, [character, healingTimer, ticksRemaining]() {
        if (*ticksRemaining > 0 && character) {
            int currentHP = character->getHP();
            int newHP = std::min(100, currentHP + 5); // 每次回复5血量，但不超过100
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
            // 角色不存在或出错，停止定时器
            healingTimer->stop();
            healingTimer->deleteLater();
            delete ticksRemaining;
        }
    });
    
    // 每秒触发一次
    healingTimer->start(1000);
}
