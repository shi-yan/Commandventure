#ifndef TAB_WIDGET
#define TAB_WIDGET

#include <QTabWidget>
#include <QMap>
#include "docktabwidget.h"

#include "properties.h"

class TermWidgetHolder;
class QAction;
class QActionGroup;


class TabWidget : public DockTabWidget
{
Q_OBJECT
public:
    TabWidget(QWidget* parent = 0);

    TermWidgetHolder * terminalHolder();

    void showHideTabBar();

public slots:
    int addNewTab(const QString& shell_program = QString());
    void removeTab(int);
    void removeCurrentTab();
    int switchToRight();
    int switchToLeft();
    void removeFinished();
    void moveLeft();
    void moveRight();
    void renameSession(int);
    void renameCurrentSession();
    void setWorkDirectory(const QString&);

    void switchNextSubterminal();
    void switchPrevSubterminal();
    void splitHorizontally();
    void splitVertically();
    void splitCollapse();

    void copySelection();
    void pasteClipboard();
    void pasteSelection();
    void zoomIn();
    void zoomOut();
    void zoomReset();

    void changeTabPosition(QAction *);
    void changeScrollPosition(QAction *);
    void changeKeyboardCursorShape(QAction *);
    void propertiesChanged();

    void clearActiveTerminal();

    void saveSession();
    void loadSession();

    void preset2Horizontal();
    void preset2Vertical();
    void preset4Terminals();

signals:
    void closeTabNotification();
    void tabRenameRequested(int);
    void currentTitleChanged(int);

protected:
    enum Direction{Left = 1, Right};
    void contextMenuEvent(QContextMenuEvent * event);
    void move(Direction);
    /*! Event filter for TabWidget's QTabBar. It's installed on tabBar()
        in the constructor.
        It's purpose is to handle doubleclicks on QTabBar for session
        renaming or new tab opening
     */
    bool eventFilter(QObject *obj, QEvent *event);

    DockTabWidget* createAnother(QWidget *parent) override;

protected slots:
    void updateTabIndices();
    void onTermTitleChanged(QString title, QString icon);

private:
    int tabNumerator;
    QString work_dir;
    /* re-order naming of the tabs then removeCurrentTab() */
    void renameTabsAfterRemove();
};

#endif
