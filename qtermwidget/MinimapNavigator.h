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
    QPixmap m_historyScreenCachedImage;

    int historyHeight;
public:
    MinimapNavigator(QWidget *parent = 0);

    QPixmap *getCurrentScreenCachedImage();
    QPixmap *getHistoryScreenCachedImage();

    void scrollImage(int dx, int dy, QRect &region);

protected:
    void paintEvent(QPaintEvent*);
};

#endif // MINIMAPNAVIGATOR_H
