#ifndef QT_PROGRAMMING_2024_MEDICINE_H
#define QT_PROGRAMMING_2024_MEDICINE_H

#include "../Mountable.h"
#include <QString>

class Medicine : public Mountable {
public:
    explicit Medicine(QGraphicsItem *parent = nullptr, const QString &pixmapPath = "", const QString &medicineName = "");
    
    QString getMedicineName() const { return medicineName; }
    
    // 纯虚函数，由子类实现具体的治疗效果
    virtual void applyEffect(class Character* character) = 0;

protected:
    QString medicineName;
};

#endif // QT_PROGRAMMING_2024_MEDICINE_H
