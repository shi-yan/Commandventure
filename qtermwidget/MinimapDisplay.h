#ifndef MINIMAPDISPLAY_H
#define MINIMAPDISPLAY_H

// Qt
#include <QColor>
#include <QPointer>
#include <QWidget>
#include <QPainter>

// Konsole
#include "Filter.h"
#include "Character.h"
#include "qtermwidget.h"

#include "TerminalDisplay.h"

class QDrag;
class QDragEnterEvent;
class QDropEvent;
class QLabel;
class QTimer;
class QEvent;
class QGridLayout;
class QKeyEvent;
class QScrollBar;
class QShowEvent;
class QHideEvent;
class QTimerEvent;
class QWidget;
class ScreenWindow;

/**
 * A widget which displays output from a terminal emulation and sends input keypresses and mouse activity
 * to the terminal.
 *
 * When the terminal emulation receives new output from the program running in the terminal,
 * it will update the display by calling updateImage().
 *
 * TODO More documentation
 */
class MinimapDisplay : public TerminalDisplay
{
   Q_OBJECT

public:
    /** Constructs a new terminal display widget with the specified parent. */
    MinimapDisplay(QWidget *parent=0);
    virtual ~MinimapDisplay();

};

#endif // MINIMAPDISPLAY_H
