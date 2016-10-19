#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T14:29:34
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Micemallow
TEMPLATE = app

DEFINES += HAVE_POSIX_OPENPT


SOURCES += main.cpp\
        mainwindow.cpp \
    bookmarkswidget.cpp \
    fontdialog.cpp \
    properties.cpp \
    propertiesdialog.cpp \
    tabwidget.cpp \
    termwidget.cpp \
    termwidgetholder.cpp \
#    third-party/qxtglobalshortcut_mac.cpp \
#    third-party/qxtglobalshortcut_win.cpp \
    third-party/qxtglobalshortcut_x11.cpp \
    third-party/qxtglobalshortcut.cpp \
    qtermwidget/BlockArray.cpp \
    qtermwidget/ColorScheme.cpp \
    qtermwidget/Emulation.cpp \
    qtermwidget/Filter.cpp \
    qtermwidget/History.cpp \
    qtermwidget/HistorySearch.cpp \
    qtermwidget/KeyboardTranslator.cpp \
    qtermwidget/konsole_wcwidth.cpp \
    qtermwidget/kprocess.cpp \
    qtermwidget/kpty.cpp \
    qtermwidget/kptydevice.cpp \
    qtermwidget/kptyprocess.cpp \
    qtermwidget/Pty.cpp \
    qtermwidget/qtermwidget.cpp \
    qtermwidget/Screen.cpp \
    qtermwidget/ScreenWindow.cpp \
    qtermwidget/SearchBar.cpp \
    qtermwidget/Session.cpp \
    qtermwidget/ShellCommand.cpp \
    qtermwidget/TerminalCharacterDecoder.cpp \
    qtermwidget/TerminalDisplay.cpp \
    qtermwidget/tools.cpp \
    qtermwidget/Vt102Emulation.cpp \
    qtermwidget/StatusBar.cpp \
    docktabwidget.cpp

HEADERS  += mainwindow.h \
    bookmarkswidget.h \
    config.h \
    fontdialog.h \
    properties.h \
    propertiesdialog.h \
    tabwidget.h \
    termwidget.h \
    termwidgetholder.h \
    third-party/qxtglobal.h \
    third-party/qxtglobalshortcut_p.h \
    third-party/qxtglobalshortcut.h \
    qtermwidget/BlockArray.h \
    qtermwidget/Character.h \
    qtermwidget/CharacterColor.h \
    qtermwidget/ColorScheme.h \
    qtermwidget/ColorTables.h \
    qtermwidget/DefaultTranslatorText.h \
    qtermwidget/Emulation.h \
    qtermwidget/ExtendedDefaultTranslator.h \
    qtermwidget/Filter.h \
    qtermwidget/History.h \
    qtermwidget/HistorySearch.h \
    qtermwidget/KeyboardTranslator.h \
    qtermwidget/konsole_wcwidth.h \
    qtermwidget/kprocess.h \
    qtermwidget/kpty_p.h \
    qtermwidget/kpty.h \
    qtermwidget/kptydevice.h \
    qtermwidget/kptyprocess.h \
    qtermwidget/LineFont.h \
    qtermwidget/Pty.h \
    qtermwidget/qtermwidget.h \
    qtermwidget/Screen.h \
    qtermwidget/ScreenWindow.h \
    qtermwidget/SearchBar.h \
    qtermwidget/Session.h \
    qtermwidget/ShellCommand.h \
    qtermwidget/TerminalCharacterDecoder.h \
    qtermwidget/TerminalDisplay.h \
    qtermwidget/tools.h \
    qtermwidget/Vt102Emulation.h \
    qtermwidget/StatusBar.h \
    docktabwidget.h

RESOURCES += \
    icons.qrc

DISTFILES += \
    Micemallow.pro.user \
    qtermwidget/LineFont.src \
    qtermwidget/default.keytab \
    qtermwidget/README

FORMS += \
    qtermwidget/SearchBar.ui \
    forms/bookmarkswidget.ui \
    forms/fontdialog.ui \
    forms/propertiesdialog.ui \
    forms/qterminal.ui \
    qtermwidget/StatusBar.ui

LIBS += -lX11
