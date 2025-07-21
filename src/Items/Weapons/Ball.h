#ifndef QT_PROGRAMMING_2024_BALL_H
#define QT_PROGRAMMING_2024_BALL_H

#include "Weapon.h"

class Ball : public Weapon {
public:
    explicit Ball(QGraphicsItem *parent = nullptr);
    
    void mountToParent() override;
    void advance(int phase) override;
    
    // ��ȡ��������
    QString getWeaponName() const { return "Ball"; }
    
    // ��ȡǦ������ĵ�λ��
    QPointF getSceneCenter() const;
    
    // �����ײ
    void checkCollisions();
    
    // ���ó�ʼ�ٶ�
    void setVelocity(qreal vx, qreal vy);
    
    // ����ΪͶ��ģʽ��ʹ�ø����������
    void setThrownMode(bool thrown = true);
    
    // ����
    QGraphicsItem* shooter = nullptr;
    
    bool isMounted() const;
    
    // ��ȫɾ������
    void deleteLater();
private:
    QPointF velocity;
    static constexpr qreal gravity = 0.02; // Ĭ���������ٶȣ�����������
    static constexpr qreal thrownGravity = 1.6; // Ͷ���������������ٶ�
    qint64 creationTime; // ����ʱ��
    qint64 groundTimer = 0; // �����ʱ���������10����ʧ��ʱ
    static constexpr qint64 maxLifetime = 60000; // �������ʱ��60�루��ֹ���ڿ��У�
    bool active = true; // ���Ǧ���Ƿ������˶�
    bool isThrown = false; // �Ƿ�ΪͶ��ģʽ
};

#endif
