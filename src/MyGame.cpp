//
// Created by gerw on 8/20/24.
//
#include "MyGame.h"
#include "Scenes/BattleScene.h"

MyGame::MyGame(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 如果你在UI里放了QGraphicsView控件（如objectName为graphicsView），可直接用：
    battleScene = new BattleScene(this);
    ui->graphicsView->setScene(battleScene);
    ui->graphicsView->setFixedSize((int) battleScene->width(), (int) battleScene->height());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setFocus();

    // 如果你没有用UI里的graphicsView控件，保留原有view逻辑
    // setCentralWidget(view);
    // setFixedSize(view->sizeHint());

    battleScene->startLoop();
}
