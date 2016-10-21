#ifndef HISTORYMINIMAP_H
#define HISTORYMINIMAP_H

#include <QObject>
#include "Character.h"

class HistoryMinimap : public QObject
{
private:
    //QImage m_historyMinimap;

public:
    HistoryMinimap();

    void pushLine(QVector<Konsole::Character > &line);

};

#endif // HISTORYMINIMAP_H
