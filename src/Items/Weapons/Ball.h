#ifndef QT_PROGRAMMING_2024_BALL_H
#define QT_PROGRAMMING_2024_BALL_H

#include "Weapon.h"

class Ball : public Weapon {
public:
    explicit Ball(QGraphicsItem *parent = nullptr);
    
    void mountToParent() override;
    void advance(int phase) override;
    
    // 获取武器名称
    QString getWeaponName() const { return "Ball"; }
    
    // 获取铅球的中心点位置
    QPointF getSceneCenter() const;
    
    // 检查碰撞
    void checkCollisions();
    
    // 设置初始速度
    void setVelocity(qreal vx, qreal vy);
    
    // 设置为投掷模式（使用更快的重力）
    void setThrownMode(bool thrown = true);
    
    // 射手
    QGraphicsItem* shooter = nullptr;
    
    bool isMounted() const;
    
    // 安全删除函数
    void deleteLater();
private:
    QPointF velocity;
    static constexpr qreal gravity = 0.02; // 默认重力加速度（掉落武器）
    static constexpr qreal thrownGravity = 1.6; // 投掷武器的重力加速度
    qint64 creationTime; // 创建时间
    qint64 groundTimer = 0; // 球落地时间戳，用于10秒消失计时
    static constexpr qint64 maxLifetime = 60000; // 最大生存时间60秒（防止球卡在空中）
    bool active = true; // 标记铅球是否仍在运动
    bool isThrown = false; // 是否为投掷模式
};

#endif
