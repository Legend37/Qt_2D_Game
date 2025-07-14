//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_MYGAME_H
#define QT_PROGRAMMING_2024_MYGAME_H

#include <QGraphicsView>
#include <QMainWindow>
#include "Scenes/Scene.h"
#include "ui_mainwindow.h"


class MyGame : public QMainWindow {
    Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);

private:
    Ui::MainWindow *ui;
    Scene *battleScene;
    QGraphicsView *view;
};


#endif //QT_PROGRAMMING_2024_MYGAME_H
