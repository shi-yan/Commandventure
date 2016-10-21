#include "StatusBar.h"
#include "ui_StatusBar.h"
#include <QFontDatabase>

StatusBar::StatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBar)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/iconfont/icomoon.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont iconfont(family);
    connect(ui->openFolderPushButton, SIGNAL(clicked(bool)),this,SLOT(onOpenClicked(bool)));

    ui->openFolderPushButton->setFont(iconfont);
    ui->prevPushButton->setFont(iconfont);
    ui->nextPushButton->setFont(iconfont);
    ui->logPushButton->setFont(iconfont);
}

StatusBar::~StatusBar()
{
    delete ui;
}

void StatusBar::onOpenClicked(bool)
{
    emit openCurrentLocation();
}
