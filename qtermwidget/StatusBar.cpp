#include "StatusBar.h"
#include "ui_StatusBar.h"

StatusBar::StatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBar)
{
    ui->setupUi(this);

    connect(ui->openFolderPushButton, SIGNAL(clicked(bool)),this,SLOT(onOpenClicked(bool)));
}

StatusBar::~StatusBar()
{
    delete ui;
}

void StatusBar::onOpenClicked(bool)
{
    emit openCurrentLocation();
}
