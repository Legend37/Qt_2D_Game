#ifndef QT_PROGRAMMING_2024_MEDICINE_H
#define QT_PROGRAMMING_2024_MEDICINE_H

#include "../Mountable.h"
#include <QString>
#include <QPainter>

class Medicine : public Mountable {
public:
    explicit Medicine(QGraphicsItem *parent = nullptr, const QString &pixmapPath = "", const QString &medicineName = "");
    
    QString getMedicineName() const { return medicineName; }
    
    // ���麯����������ʵ�־��������Ч��
    virtual void applyEffect(class Character* character) = 0;
    
    // ��д���Ʒ�������Ӻ�ɫ�߿�
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QString medicineName;
};

#endif // QT_PROGRAMMING_2024_MEDICINE_H
