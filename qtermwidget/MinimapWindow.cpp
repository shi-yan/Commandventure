/*
    Copyright (C) 2007 by Robert Knight <robertknight@gmail.com>

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
#include "MinimapWindow.h"

// Qt
#include <QtDebug>

// Konsole
#include "Screen.h"

MinimapWindow::MinimapWindow(QObject* parent)
    : QObject(parent)
    , _windowBuffer(0)
    , _windowBufferSize(0)
    , _bufferNeedsUpdate(true)
    , _windowLines(1)
    , _currentLine(0)
    , _trackOutput(true)
    , _scrollCount(0)
{
}

MinimapWindow::~MinimapWindow()
{
    delete[] _windowBuffer;
}

void MinimapWindow::setScreen(Screen* screen)
{
    Q_ASSERT( screen );

    _screen = screen;
}

Screen* MinimapWindow::screen() const
{
    return _screen;
}

Character* MinimapWindow::getImage()
{
    // reallocate internal buffer if the window size has changed
    int size = windowLines() * windowColumns();
    if (_windowBuffer == 0 || _windowBufferSize != size)
    {
        delete[] _windowBuffer;
        _windowBufferSize = size;
        _windowBuffer = new Character[size];
        _bufferNeedsUpdate = true;
    }

     if (!_bufferNeedsUpdate)
        return _windowBuffer;

    _screen->getImage(_windowBuffer, size,
                      currentLine(), endWindowLine());

    // this window may look beyond the end of the screen, in which
    // case there will be an unused area which needs to be filled
    // with blank characters
    fillUnusedArea();

    _bufferNeedsUpdate = false;
    return _windowBuffer;
}

void MinimapWindow::fillUnusedArea()
{
    int screenEndLine = _screen->getHistLines() + _screen->getLines() - 1;
    int windowEndLine = currentLine() + windowLines() - 1;

    int unusedLines = windowEndLine - screenEndLine;
    int charsToFill = unusedLines * windowColumns();

    Screen::fillWithDefaultChar(_windowBuffer + _windowBufferSize - charsToFill,charsToFill);
}

// return the index of the line at the end of this window, or if this window
// goes beyond the end of the screen, the index of the line at the end
// of the screen.
//
// when passing a line number to a Screen method, the line number should
// never be more than endWindowLine()
//
int MinimapWindow::endWindowLine() const
{
    return qMin(currentLine() + windowLines() - 1,
                lineCount() - 1);
}

QVector<LineProperty> MinimapWindow::getLineProperties()
{
    QVector<LineProperty> result = _screen->getLineProperties(currentLine(),endWindowLine());

    if (result.count() != windowLines())
        result.resize(windowLines());

    return result;
}

QString MinimapWindow::selectedText( bool preserveLineBreaks ) const
{
    return _screen->selectedText( preserveLineBreaks );
}

void MinimapWindow::getSelectionStart( int& column , int& line )
{
    _screen->getSelectionStart(column,line);
    line -= currentLine();
}

void MinimapWindow::getSelectionEnd( int& column , int& line )
{
    _screen->getSelectionEnd(column,line);
    line -= currentLine();
}

void MinimapWindow::setSelectionStart( int column , int line , bool columnMode )
{
    _screen->setSelectionStart( column , qMin(line + currentLine(),endWindowLine())  , columnMode);

    _bufferNeedsUpdate = true;
    emit selectionChanged();
}

void MinimapWindow::setSelectionEnd( int column , int line )
{
    _screen->setSelectionEnd( column , qMin(line + currentLine(),endWindowLine()) );

    _bufferNeedsUpdate = true;
    emit selectionChanged();
}

bool MinimapWindow::isSelected( int column , int line )
{
    return _screen->isSelected( column , qMin(line + currentLine(),endWindowLine()) );
}

void MinimapWindow::clearSelection()
{
    _screen->clearSelection();

    emit selectionChanged();
}

void MinimapWindow::setWindowLines(int lines)
{
    Q_ASSERT(lines > 0);
    _windowLines = lines;
}

int MinimapWindow::windowLines() const
{
    return _windowLines;
}

int MinimapWindow::windowColumns() const
{
    return _screen->getColumns();
}

int MinimapWindow::lineCount() const
{
    return _screen->getHistLines() + _screen->getLines();
}

int MinimapWindow::columnCount() const
{
    return _screen->getColumns();
}

QPoint MinimapWindow::cursorPosition() const
{
    QPoint position;

    position.setX( _screen->getCursorX() );
    position.setY( _screen->getCursorY() );

    return position;
}

int MinimapWindow::currentLine() const
{
    return qBound(0,_currentLine,lineCount()-windowLines());
}

void MinimapWindow::scrollBy( RelativeScrollMode mode , int amount )
{
    if ( mode == ScrollLines )
    {
        scrollTo( currentLine() + amount );
    }
    else if ( mode == ScrollPages )
    {
        scrollTo( currentLine() + amount * ( windowLines() / 2 ) );
    }
}

bool MinimapWindow::atEndOfOutput() const
{
    return currentLine() == (lineCount()-windowLines());
}

void MinimapWindow::scrollTo( int line )
{
    int maxCurrentLineNumber = lineCount() - windowLines();
    line = qBound(0,line,maxCurrentLineNumber);

    const int delta = line - _currentLine;
    _currentLine = line;

    // keep track of number of lines scrolled by,
    // this can be reset by calling resetScrollCount()
    _scrollCount += delta;

    _bufferNeedsUpdate = true;

    emit scrolled(_currentLine);
}

void MinimapWindow::setTrackOutput(bool trackOutput)
{
    _trackOutput = trackOutput;
}

bool MinimapWindow::trackOutput() const
{
    return _trackOutput;
}

int MinimapWindow::scrollCount() const
{
    return _scrollCount;
}

void MinimapWindow::resetScrollCount()
{
    _scrollCount = 0;
}

QRect MinimapWindow::scrollRegion() const
{
    bool equalToScreenSize = windowLines() == _screen->getLines();

    if ( atEndOfOutput() && equalToScreenSize )
        return _screen->lastScrolledRegion();
    else
        return QRect(0,0,windowColumns(),windowLines());
}

void MinimapWindow::notifyOutputChanged()
{
    // move window to the bottom of the screen and update scroll count
    // if this window is currently tracking the bottom of the screen
    if ( _trackOutput )
    {
        _scrollCount -= _screen->scrolledLines();
        _currentLine = qMax(0,_screen->getHistLines() - (windowLines()-_screen->getLines()));
    }
    else
    {
        // if the history is not unlimited then it may
        // have run out of space and dropped the oldest
        // lines of output - in this case the screen
        // window's current line number will need to
        // be adjusted - otherwise the output will scroll
        _currentLine = qMax(0,_currentLine -
                              _screen->droppedLines());

        // ensure that the screen window's current position does
        // not go beyond the bottom of the screen
        _currentLine = qMin( _currentLine , _screen->getHistLines() );
    }

    _bufferNeedsUpdate = true;

    emit outputChanged();
}

//#include "ScreenWindow.moc"
