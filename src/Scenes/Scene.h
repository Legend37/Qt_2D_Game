//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_SCENE_H
#define QT_PROGRAMMING_2024_SCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QRectF>
#include <QTimer>

class Scene : public QGraphicsScene {
Q_OBJECT

public:
    explicit Scene(QObject *parent);

    void startLoop();

    virtual void processInput();

    virtual void processMovement();

    virtual void processPicking();

protected slots:

    virtual void update();

protected:
    // 允许子类重载以自定义前景绘制（如HP条）
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    qint64 deltaTime{};

private:
    QTimer *timer;
    qint64 lastTime{-1};
};


#endif //QT_PROGRAMMING_2024_SCENE_H
