#ifndef MINIMAPNAVIGATOR_H
#define MINIMAPNAVIGATOR_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include "Character.h"

class MinimapNavigator : public QWidget
{
private:
    QImage m_minimapImage;
    QPainter m_painter;
    unsigned int m_lineCount;
    QString text;

public:
    MinimapNavigator(QWidget *parent = 0);


    void pushLine(QVector<Konsole::Character> &c);
};

#endif // MINIMAPNAVIGATOR_H
