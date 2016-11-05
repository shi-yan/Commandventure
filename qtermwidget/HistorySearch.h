#ifndef TASK_H
#define	TASK_H

#include <QObject>
#include <QPointer>
#include <QMap>

#include <qtermwidget/Session.h>
#include <qtermwidget/ScreenWindow.h>

#include "Emulation.h"
#include "TerminalCharacterDecoder.h"

typedef QPointer<Emulation> EmulationPtr;

class HistorySearch : public QObject
{
    Q_OBJECT

public:
    explicit HistorySearch(EmulationPtr emulation, QRegExp regExp, bool forwards,
                           int startColumn, int startLine, QObject* parent);

    ~HistorySearch();

    void search();

signals:
    void matchFound(int startColumn, int startLine, int endColumn, int endLine);
    void noMatchFound();

private:
    bool search(int startColumn, int startLine, int endColumn, int endLine);
    int findLineNumberInString(QList<int> linePositions, int position);


    EmulationPtr m_emulation;
    QRegExp m_regExp;
    bool m_forwards;
    int m_startColumn;
    int m_startLine;

    int m_foundStartColumn;
    int m_foundStartLine;
    int m_foundEndColumn;
    int m_foundEndLine;
};

#endif	/* TASK_H */

