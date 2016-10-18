#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>

namespace Ui {
class StatusBar;
}

class StatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = 0);
    ~StatusBar();

signals:
    void openCurrentLocation();

private:
    Ui::StatusBar *ui;

private slots:
    void onOpenClicked(bool);
};

#endif // STATUSBAR_H
