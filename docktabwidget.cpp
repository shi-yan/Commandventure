#include <QtGui>
#include <QApplication>

#include "docktabwidget.h"
#include "mainwindow.h"

#define MIMETYPE_TABINDEX "x-paintfield-tabindex"

DockTabWidget::DockTabWidget(QWidget *parent) :
	QTabWidget(parent)
{
	setTabBar(new DockTabBar(this));
	setDocumentMode(true);
	setAcceptDrops(true);
}

DockTabWidget::DockTabWidget(DockTabWidget *other, QWidget *parent) :
	DockTabWidget(parent)
{
	setFloatingBaseWidget(other->floatingBaseWidget());
	setFloatingEnabled(other->isFloatingEnabled());
	resize(other->size());
}

void DockTabWidget::setFloatingBaseWidget(QWidget *widget)
{
	_floatingBaseWidget = widget;
	if (_floatingEnabled && parentWidget() == 0)
		setParent(widget);
}

void DockTabWidget::setFloatingEnabled(bool x)
{
	_floatingEnabled = x;
	
	if (parent() == 0)
	{
		if (x)
			setWindowFlags(Qt::Tool);
		else
			setWindowFlags(Qt::Window);
	}
}

void DockTabWidget::moveTab(DockTabWidget *source, int sourceIndex, DockTabWidget *dest, int destIndex)
{
	if (source == dest && sourceIndex < destIndex)
		destIndex--;
	
	QWidget *widget = source->widget(sourceIndex);
	QString text = source->tabText(sourceIndex);
	
	source->removeTab(sourceIndex);
	
	dest->insertTab(destIndex, widget, text);
	dest->setCurrentIndex(destIndex);
}

void DockTabWidget::decodeTabDropEvent(QDropEvent *event, DockTabWidget **p_tabWidget, int *p_index)
{
	DockTabBar *tabBar = qobject_cast<DockTabBar *>(event->source());
	if (!tabBar)
	{
		*p_tabWidget = nullptr;
		*p_index = 0;
		return;
	}
	
	QByteArray data = event->mimeData()->data(MIMETYPE_TABINDEX);
	QDataStream stream(&data, QIODevice::ReadOnly);
	
	int index;
	stream >> index;
	
	*p_tabWidget = tabBar->tabWidget();
	*p_index = index;
}

bool DockTabWidget::eventIsTabDrag(QDragEnterEvent *event)
{
	return event->mimeData()->hasFormat(MIMETYPE_TABINDEX) && qobject_cast<DockTabBar *>(event->source());
}

void DockTabWidget::deleteIfEmpty()
{
	if (count() == 0)
	{
		emit willBeAutomaticallyDeleted(this);
		deleteLater();
	}
}

DockTabWidget *DockTabWidget::createAnother(QWidget *parent)
{
    MainWindow *mainWindow = new MainWindow("~",
                           QString(),
                           false);

    mainWindow->consoleTabulator->deleteLater();

    mainWindow->consoleTabulator = new TabWidget(mainWindow->centralWidget());


    mainWindow->consoleTabulator->setObjectName(QStringLiteral("consoleTabulator"));
    mainWindow->consoleTabulator->setMinimumSize(QSize(320, 200));
    mainWindow->consoleTabulator->setStyleSheet(QLatin1String("QTabWidget{\n"
"    \n"
"	color: rgb(255, 0, 4);\n"
"	background-color: rgb(48, 51, 61);\n"
"    border: 1px solid #C4C4C3;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
"                                stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
"    border: 2px solid #C4C4C3;\n"
"    border-bottom-color: #C2C7CB; /* same as the pane color */\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"    min-width: 8ex;\n"
"    padding: 2px;\n"
"}\n"
"\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                stop: 0 #fafafa, stop: 0.4 #f4f4f4,\n"
"                                stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);\n"
"}"));
    mainWindow->consoleTabulator->setTabShape(QTabWidget::Rounded);
    mainWindow->consoleTabulator->setElideMode(Qt::ElideNone);

    mainWindow->gridLayout->addWidget(mainWindow->consoleTabulator, 0, 0, 1, 1);

    mainWindow->show();

    return mainWindow->consoleTabulator;

}

bool DockTabWidget::isInsertable(QWidget *widget)
{
	Q_UNUSED(widget)
	return true;
}


DockTabBar::DockTabBar(DockTabWidget *tabWidget, QWidget *parent) :
	QTabBar(parent),
	_tabWidget(tabWidget)
{
	setAcceptDrops(true);
}

int DockTabBar::insertionIndexAt(const QPoint &pos)
{
	int index = count();
	for (int i = 0; i < count(); ++i)
	{
		QRect rect = tabRect(i);
		QRect rect1(rect.x(), rect.y(), rect.width() / 2, rect.height());
		QRect rect2(rect.x() + rect1.width(), rect.y(), rect.width() - rect1.width(), rect.height());
		if (rect1.contains(pos))
		{
			index = i;
			break;
		}
		if (rect2.contains(pos))
		{
			index = i + 1;
			break;
		}
	}
	return index;
}

void DockTabBar::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_dragStartPos = event->pos();
		_isStartingDrag = true;
	}
	QTabBar::mousePressEvent(event);
}

void DockTabBar::mouseMoveEvent(QMouseEvent *event)
{
	if (!_isStartingDrag)
		return;
	
	if (!event->buttons() & Qt::LeftButton)
		return;
	
	if ((event->pos() - _dragStartPos).manhattanLength() < QApplication::startDragDistance())
		return;
	
	int index = tabAt(event->pos());
	
	if (index < 0)
		return;
	
	// create data
	
	QMimeData *mimeData = new QMimeData;
	
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << index;
	
	mimeData->setData(MIMETYPE_TABINDEX, data);
	
	// create pixmap
	
	QRect rect = tabRect(index);
	QPixmap pixmap(rect.size());
	
	render(&pixmap, QPoint(), QRegion(rect));
	
	// exec drag
	
	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	QPoint offset = _dragStartPos - rect.topLeft();
	drag->setHotSpot(offset);
	Qt::DropAction dropAction = drag->exec(Qt::MoveAction | Qt::IgnoreAction);
	
	if (dropAction != Qt::MoveAction)	// drop is not accepted
	{
		DockTabWidget *newTabWidget = _tabWidget->createAnother();
		if (!newTabWidget->isInsertable(_tabWidget, index))
		{
			newTabWidget->deleteLater();
			return;
		}
		
		DockTabWidget::moveTab(_tabWidget, index, newTabWidget, 0);
		
		QRect newGeometry = newTabWidget->geometry();
		newGeometry.moveTopLeft(QCursor::pos() - offset);
		newTabWidget->setGeometry(newGeometry);
		newTabWidget->show();
	}
	
	_tabWidget->deleteIfEmpty();
	_isStartingDrag = false;
}

void DockTabBar::dragEnterEvent(QDragEnterEvent *event)
{
	if (DockTabWidget::eventIsTabDrag(event))
		event->acceptProposedAction();
}

void DockTabBar::dropEvent(QDropEvent *event)
{
	DockTabWidget *oldTabWidget;
	int oldIndex;
	DockTabWidget::decodeTabDropEvent(event, &oldTabWidget, &oldIndex);
	
	if (oldTabWidget && _tabWidget->isInsertable(oldTabWidget, oldIndex))
	{
		int newIndex = insertionIndexAt(event->pos());
		DockTabWidget::moveTab(oldTabWidget, oldIndex, _tabWidget, newIndex);
		event->acceptProposedAction();
	}
}
