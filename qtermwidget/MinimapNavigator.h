#ifndef MINIMAPNAVIGATOR_H
#define MINIMAPNAVIGATOR_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "Character.h"

class MinimapNavigator : public QWidget
{
private:
    QPixmap m_currentScreenCachedImage;


public:
    MinimapNavigator(QWidget *parent = 0);

    QPixmap *getCurrentScreenCachedImage();

protected:
    void paintEvent(QPaintEvent*);
};

#endif // MINIMAPNAVIGATOR_H
