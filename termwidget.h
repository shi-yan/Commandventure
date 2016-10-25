#ifndef TERMWIDGET_H
#define TERMWIDGET_H

#include "qtermwidget/qtermwidget.h"

#include <QAction>

class TermWidgetHolder;

class TermWidgetImpl : public QTermWidget
{
    Q_OBJECT

//        QMap< QString, QAction * > actionMap;

    public:

        TermWidgetImpl(const QString & wdir, const QString & shell=QString(), QWidget * parent=0);
        void propertiesChanged();

    signals:
        void renameSession();
        void removeCurrentSession();

    public slots:
        void zoomIn();
        void zoomOut();
        void zoomReset();

    private slots:
        void customContextMenuCall(const QPoint & pos);
        void activateUrl(const QUrl& url);
};


class TermWidget : public QWidget
{
    Q_OBJECT

    TermWidgetImpl * m_term;
    QVBoxLayout * m_layout;
    QColor m_border;
    TermWidgetHolder *m_holder;

    public:
        TermWidget(const QString & wdir, TermWidgetHolder* holder, const QString & shell=QString(),  QWidget * parent=0);

        void propertiesChanged(); 
        QStringList availableKeyBindings() { return m_term->availableKeyBindings(); }

        TermWidgetImpl * impl() { return m_term; }

    signals:
        void finished();
        void renameSession();
        void removeCurrentSession();
        void splitHorizontal(TermWidget * self);
        void splitVertical(TermWidget * self);
        void splitCollapse(TermWidget * self);
        void termGetFocus(TermWidget * self);
        void termTitleChanged(QString titleText, QString icon);

    public slots:

    protected:
        void paintEvent (QPaintEvent * event);

    private slots:
        void term_termGetFocus();
        void term_termLostFocus();
        bool hasSiblings();
};

#endif

