#ifndef VT102EMULATION_H
#define VT102EMULATION_H

// Standard Library
#include <stdio.h>

// Qt
#include <QKeyEvent>
#include <QHash>
#include <QTimer>

// Konsole
#include "Emulation.h"
#include "Screen.h"

#define MODE_AppScreen       (MODES_SCREEN+0)   // Mode #1
#define MODE_AppCuKeys       (MODES_SCREEN+1)   // Application cursor keys (DECCKM)
#define MODE_AppKeyPad       (MODES_SCREEN+2)   //
#define MODE_Mouse1000       (MODES_SCREEN+3)   // Send mouse X,Y position on press and release
#define MODE_Mouse1001       (MODES_SCREEN+4)   // Use Hilight mouse tracking
#define MODE_Mouse1002       (MODES_SCREEN+5)   // Use cell motion mouse tracking
#define MODE_Mouse1003       (MODES_SCREEN+6)   // Use all motion mouse tracking
#define MODE_Mouse1005       (MODES_SCREEN+7)   // Xterm-style extended coordinates
#define MODE_Mouse1006       (MODES_SCREEN+8)   // 2nd Xterm-style extended coordinates
#define MODE_Mouse1015       (MODES_SCREEN+9)   // Urxvt-style extended coordinates
#define MODE_Ansi            (MODES_SCREEN+10)   // Use US Ascii for character sets G0-G3 (DECANM)
#define MODE_132Columns      (MODES_SCREEN+11)  // 80 <-> 132 column mode switch (DECCOLM)
#define MODE_Allow132Columns (MODES_SCREEN+12)  // Allow DECCOLM mode
#define MODE_BracketedPaste  (MODES_SCREEN+13)  // Xterm-style bracketed paste mode
#define MODE_total           (MODES_SCREEN+14)

namespace Konsole
{

struct CharCodes
{
  // coding info
  char charset[4]; //
  int  cu_cs;      // actual charset.
  bool graphic;    // Some VT100 tricks
  bool pound  ;    // Some VT100 tricks
  bool sa_graphic; // saved graphic
  bool sa_pound;   // saved pound
};

/**
 * Provides an xterm compatible terminal emulation based on the DEC VT102 terminal.
 * A full description of this terminal can be found at http://vt100.net/docs/vt102-ug/
 *
 * In addition, various additional xterm escape sequences are supported to provide
 * features such as mouse input handling.
 * See http://rtfm.etla.org/xterm/ctlseq.html for a description of xterm's escape
 * sequences.
 *
 */
class Vt102Emulation : public Emulation
{
Q_OBJECT

public:
  /** Constructs a new emulation */
  Vt102Emulation();
  ~Vt102Emulation();

  // reimplemented from Emulation
  virtual void clearEntireScreen();
  virtual void reset();
  virtual char eraseChar() const;

public slots:
  // reimplemented from Emulation
  virtual void sendString(const char*,int length = -1);
  virtual void sendText(const QString& text);
  virtual void sendKeyEvent(QKeyEvent*);
  virtual void sendMouseEvent(int buttons, int column, int line, int eventType);
  virtual void focusLost();
  virtual void focusGained();

protected:
  // reimplemented from Emulation
  virtual void setMode(int mode);
  virtual void resetMode(int mode);
  virtual void receiveChar(int cc);

private slots:
  //causes changeTitle() to be emitted for each (int,QString) pair in pendingTitleUpdates
  //used to buffer multiple title updates
  void updateTitle();

private:
  unsigned short applyCharset(unsigned short c);
  void setCharset(int n, int cs);
  void useCharset(int n);
  void setAndUseCharset(int n, int cs);
  void saveCursor();
  void restoreCursor();
  void resetCharset(int scrno);

  void setMargins(int top, int bottom);
  //set margins for all screens back to their defaults
  void setDefaultMargins();

  // returns true if 'mode' is set or false otherwise
  bool getMode    (int mode);
  // saves the current boolean value of 'mode'
  void saveMode   (int mode);
  // restores the boolean value of 'mode'
  void restoreMode(int mode);
  // resets all modes
  // (except MODE_Allow132Columns)
  void resetModes();

  void resetTokenizer();
  #define MAX_TOKEN_LENGTH 80
  void addToCurrentToken(int cc);
  int tokenBuffer[MAX_TOKEN_LENGTH]; //FIXME: overflow?
  int tokenBufferPos;
#define MAXARGS 15
  void addDigit(int dig);
  void addArgument();
  int argv[MAXARGS];
  int argc;
  void initTokenizer();

  // Set of flags for each of the ASCII characters which indicates
  // what category they fall into (printable character, control, digit etc.)
  // for the purposes of decoding terminal output
  int charClass[256];

  void reportDecodingError();

  void processToken(int code, int p, int q);
  void processWindowAttributeChange();

  void reportTerminalType();
  void reportSecondaryAttributes();
  void reportStatus();
  void reportAnswerBack();
  void reportCursorPosition();
  void reportTerminalParms(int p);

  void onScrollLock();
  void scrollLock(const bool lock);

  // clears the screen and resizes it to the specified
  // number of columns
  void clearScreenAndSetColumns(int columnCount);

  CharCodes _charset[2];

  class TerminalState
  {
  public:
    // Initializes all modes to false
    TerminalState()
    { memset(&mode,false,MODE_total * sizeof(bool)); }

    bool mode[MODE_total];
  };

  TerminalState _currentModes;
  TerminalState _savedModes;

  //hash table and timer for buffering calls to the session instance
  //to update the name of the session
  //or window title.
  //these calls occur when certain escape sequences are seen in the
  //output from the terminal
  QHash<int,QString> _pendingTitleUpdates;
  QTimer* _titleUpdateTimer;

    bool _reportFocusEvents;
};

}

#endif // VT102EMULATION_H