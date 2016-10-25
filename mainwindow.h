#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_qterminal.h"

#include <QMainWindow>
#include "third-party/qxtglobalshortcut.h"

class QToolButton;
class TabWidget;

class MainWindow : public QMainWindow , private Ui::mainWindow
{
    Q_OBJECT

    friend class TabWidget;

public:
    MainWindow(const QString& work_dir, const QString& command,
               bool dropMode,
               QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MainWindow();

    bool dropMode() const { return m_dropMode; }
    QMenu *getContextMenu() const { return m_contextMenu; }

protected:
     bool event(QEvent* event);

private:
    QActionGroup *tabPosition, *scrollBarPosition, *keyboardCursorShape;
    QMenu *tabPosMenu, *scrollPosMenu, *keyboardCursorShapeMenu;

    QString m_initWorkDir;
    QString m_initShell;

    QDockWidget *m_bookmarksDock;
    QMenu * m_contextMenu;

    void setup_FileMenu_Actions();
    void setup_ActionsMenu_Actions();
    void setup_ViewMenu_Actions();
    void setup_ContextMenu_Actions();
    void setupCustomDirs();

    void closeEvent(QCloseEvent*);

    void enableDropMode();
    QToolButton *m_dropLockButton;
    bool m_dropMode;
    QxtGlobalShortcut m_dropShortcut;
    void realign();
    void setDropShortcut(QKeySequence dropShortCut);

private slots:
    void on_consoleTabulator_currentChanged(int);
    void propertiesChanged();
    void actAbout_triggered();
    void actProperties_triggered();
    void updateActionGroup(QAction *);

    void toggleBorderless();
    void toggleTabBar();
    void toggleMenu();

    void showFullscreen(bool fullscreen);
    void showHide();
    void setKeepOpen(bool value);
    void find();

    void newTerminalWindow();
    void bookmarksWidget_callCommand(const QString&);
    void bookmarksDock_visibilityChanged(bool visible);

    void addNewTab();
    void onCurrentTitleChanged(int index);
};
#endif //MAINWINDOW_H
