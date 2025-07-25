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

    // 设置窗口大小与场景大小相同（1280x720）
    // 考虑到窗口边框和标题栏，需要调整窗口大小
    setFixedSize(1280, 720 + 40); // 添加额外高度给标题栏和边框
    
    // 禁用最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    
    // 可选：设置窗口标题
    setWindowTitle("Qt 2D Game");

    // 如果你没有用UI里的graphicsView控件，保留原有view逻辑
    // setCentralWidget(view);
    // setFixedSize(view->sizeHint());

    battleScene->startLoop();
}
