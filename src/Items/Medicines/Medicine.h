#ifndef QT_PROGRAMMING_2024_MEDICINE_H
#define QT_PROGRAMMING_2024_MEDICINE_H

#include "../Mountable.h"
#include <QString>
#include <QPainter>

class Medicine : public Mountable {
public:
    explicit Medicine(QGraphicsItem *parent = nullptr, const QString &pixmapPath = "", const QString &medicineName = "");
    
    QString getMedicineName() const { return medicineName; }
    
    // 纯虚函数，由子类实现具体的治疗效果
    virtual void applyEffect(class Character* character) = 0;
    
    // 重写绘制方法，添加红色边框
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QString medicineName;
};

#endif // QT_PROGRAMMING_2024_MEDICINE_H
