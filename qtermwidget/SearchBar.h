#ifndef _SEARCHBAR_H
#define	_SEARCHBAR_H

#include <QRegExp>

#include "ui_SearchBar.h"
#include "HistorySearch.h"

class SearchBar : public QWidget {
    Q_OBJECT
public:
    SearchBar(QWidget* parent = 0);
    virtual ~SearchBar();
    virtual void show();
    QString searchText();
    bool useRegularExpression();
    bool matchCase();
    bool highlightAllMatches();

public slots:
    void noMatchFound();

signals:
    void searchCriteriaChanged();
    void highlightMatchesChanged(bool highlightMatches);
    void findNext();
    void findPrevious();

protected:
    virtual void keyReleaseEvent(QKeyEvent* keyEvent);

private slots:
    void clearBackgroundColor();

private:
    Ui::SearchBar widget;
    QAction *m_matchCaseMenuEntry;
    QAction *m_useRegularExpressionMenuEntry;
    QAction *m_highlightMatchesMenuEntry;
};

#endif	/* _SEARCHBAR_H */
