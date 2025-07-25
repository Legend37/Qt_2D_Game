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

    // ���ô��ڴ�С�볡����С��ͬ��1280x720��
    // ���ǵ����ڱ߿�ͱ���������Ҫ�������ڴ�С
    setFixedSize(1280, 720 + 40); // ��Ӷ���߶ȸ��������ͱ߿�
    
    // ������󻯰�ť
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    
    // ��ѡ�����ô��ڱ���
    setWindowTitle("Qt 2D Game");

    // �����û����UI���graphicsView�ؼ�������ԭ��view�߼�
    // setCentralWidget(view);
    // setFixedSize(view->sizeHint());

    battleScene->startLoop();
}
