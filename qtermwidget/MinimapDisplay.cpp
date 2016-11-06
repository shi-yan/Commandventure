/*
    This file is part of Konsole, a terminal emulator for KDE.

    Copyright 2006-2008 by Robert Knight <robertknight@gmail.com>
    Copyright 1997,1998 by Lars Doelle <lars.doelle@on-line.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

// Own
#include "MinimapDisplay.h"

// Qt
#include <QApplication>
#include <QBoxLayout>
#include <QClipboard>
#include <QKeyEvent>
#include <QEvent>
#include <QTime>
#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QPixmap>
#include <QScrollBar>
#include <QStyle>
#include <QTimer>
#include <QtDebug>
#include <QUrl>
#include <QMimeData>
#include <QDrag>

// KDE
//#include <kshell.h>
//#include <KColorScheme>
//#include <KCursor>
//#include <kdebug.h>
//#include <KLocale>
//#include <KMenu>
//#include <KNotification>
//#include <KGlobalSettings>
//#include <KShortcut>
//#include <KIO/NetAccess>

// Konsole
//#include <config-apps.h>
#include "Filter.h"
#include "konsole_wcwidth.h"
#include "ScreenWindow.h"
#include "TerminalCharacterDecoder.h"


MinimapDisplay::MinimapDisplay(TerminalDisplay *terminalDisplay, QWidget *parent)
:TerminalDisplay(NULL, parent), _terminalDisplay(terminalDisplay)
{

}

MinimapDisplay::~MinimapDisplay()
{

}

void MinimapDisplay::calcGeometry()
{
    _terminalDisplay->calcGeometry();
  _leftMargin = DEFAULT_LEFT_MARGIN;
  _displayWidth = contentsRect().width()  - 2 * DEFAULT_LEFT_MARGIN;


  _topMargin = DEFAULT_TOP_MARGIN;
  _displayHeight = contentsRect().height() - 2 * DEFAULT_TOP_MARGIN + /* mysterious */ 1;

  _contentWidth = _terminalDisplay->_contentWidth;

  float ratio = (float) _terminalDisplay->_contentWidth / (float) _displayWidth;

  _contentHeight = ratio * _displayHeight;

  qDebug() <<"minimap display:"<< _contentWidth << _contentHeight;

  if (!_isFixedSize)
  {
     // ensure that display is always at least one column wide
     _columns = qMax(1,_contentWidth / _fontWidth);
     _usedColumns = qMin(_usedColumns,_columns);

     // ensure that display is always at least one line high
     _lines = qMax(1,_contentHeight / _fontHeight);
     _usedLines = qMin(_usedLines,_lines);
  }

  if (m_cachedPixmap.width() != _contentWidth || m_cachedPixmap.height() != _contentHeight)
  {
      qDebug() << m_cachedPixmap.width() << m_cachedPixmap.height();
      m_cachedPixmap = QPixmap(_contentWidth, _contentHeight);
  }
}

void MinimapDisplay::paintEvent( QPaintEvent* pe )
{
    //QPainter painter(this);

    QPainter cachedImagePainter(&m_cachedPixmap);
    cachedImagePainter.setFont(font());

    QRect rect2(0,0, this->_contentWidth, this->_contentHeight);

  QVector<QRect> updateRects =  (pe->region() & rect2).rects();

  foreach (const QRect &rect, updateRects)
  {
    drawBackground(cachedImagePainter,rect,QColor(48,51,61),
                    true /* use opacity setting */);



    drawContents(cachedImagePainter, rect);
    /*paint.setPen(QColor(Qt::red));
    paint.drawRect(rect);*/
    qDebug() << "repaint rect" << rect;
  }
  //drawInputMethodPreeditString(cachedImagePainter,preeditRect());
  //paintFilters(cachedImagePainter);

  //QPainter paintWidget(this);
  //m_cachedPixmap.save("test2.png");

/*  foreach (const QRect &rect, updateRects)
  {
      paintWidget.drawPixmap(rect, *m_miniMap->getCurrentScreenCachedImage(), rect);
  }
*/
 // paintWidget.drawPixmap(preeditRect(), *m_miniMap->getCurrentScreenCachedImage(), preeditRect());

  //m_miniMap->update();


  QPainter painter(this);

  QRect sdestRect(0,0,contentsRect().width(),  m_cachedPixmap.height() * (float)contentsRect().width() / (float) m_cachedPixmap.width());

  QPixmap shrink = m_cachedPixmap.scaled(sdestRect.size(),Qt::KeepAspectRatio);

  painter.drawPixmap(sdestRect, shrink, shrink.rect());

}

