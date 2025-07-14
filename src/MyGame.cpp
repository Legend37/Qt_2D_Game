//
// Created by gerw on 8/20/24.
//
#include "MyGame.h"
#include "Scenes/BattleScene.h"

MyGame::MyGame(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // �������UI�����QGraphicsView�ؼ�����objectNameΪgraphicsView������ֱ���ã�
    battleScene = new BattleScene(this);
    ui->graphicsView->setScene(battleScene);
    ui->graphicsView->setFixedSize((int) battleScene->width(), (int) battleScene->height());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setFocus();

    // �����û����UI���graphicsView�ؼ�������ԭ��view�߼�
    // setCentralWidget(view);
    // setFixedSize(view->sizeHint());

    battleScene->startLoop();
}
