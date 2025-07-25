
#include <QDateTime>
#include <QPainter>
#include <QRectF>
#include "Scene.h"

// 允许子类重载的空实现
void Scene::drawForeground(QPainter *painter, const QRectF &rect) {
    QGraphicsScene::drawForeground(painter, rect);
}

#include <QDateTime>
#include "Scene.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), timer(new QTimer(this)) {
    connect(timer, &QTimer::timeout, this, &Scene::update);
}

void Scene::update() {
    auto currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (lastTime == -1) { // first frame
        deltaTime = 0;
    } else {
        deltaTime = currentTime - lastTime;
    }
    lastTime = currentTime;

    processInput();
    processMovement();
    processPicking();
}

void Scene::startLoop() {
    timer->start(1000 / 60); //30fps
}

void Scene::processInput() {

}

void Scene::processMovement() {

}

void Scene::processPicking() {

}

