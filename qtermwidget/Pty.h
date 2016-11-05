#ifndef PTY_H
#define PTY_H

// Qt
#include <QStringList>
#include <QVector>
#include <QList>
#include <QSize>

// KDE
#include "kptyprocess.h"

/**
 * The Pty class is used to start the terminal process,
 * send data to it, receive data from it and manipulate
 * various properties of the pseudo-teletype interface
 * used to communicate with the process.
 *
 * To use this class, construct an instance and connect
 * to the sendData slot and receivedData signal to
 * send data to or receive data from the process.
 *
 * To start the terminal process, call the start() method
 * with the program name and appropriate arguments.
 */
class Pty: public KPtyProcess
{
Q_OBJECT

  public:

    /**
     * Constructs a new Pty.
     *
     * Connect to the sendData() slot and receivedData() signal to prepare
     * for sending and receiving data from the terminal process.
     *
     * To start the terminal process, call the run() method with the
     * name of the program to start and appropriate arguments.
     */
    explicit Pty(QObject* parent = 0);

    /**
     * Construct a process using an open pty master.
     * See KPtyProcess::KPtyProcess()
     */
    explicit Pty(int ptyMasterFd, QObject* parent = 0);

    ~Pty();

    /**
     * Starts the terminal process.
     *
     * Returns 0 if the process was started successfully or non-zero
     * otherwise.
     *
     * @param program Path to the program to start
     * @param arguments Arguments to pass to the program being started
     * @param environment A list of key=value pairs which will be added
     * to the environment for the new process.  At the very least this
     * should include an assignment for the TERM environment variable.
     * @param winid Specifies the value of the WINDOWID environment variable
     * in the process's environment.
     * @param addToUtmp Specifies whether a utmp entry should be created for
     * the pty used.  See K3Process::setUsePty()
     * @param dbusService Specifies the value of the KONSOLE_DBUS_SERVICE
     * environment variable in the process's environment.
     * @param dbusSession Specifies the value of the KONSOLE_DBUS_SESSION
     * environment variable in the process's environment.
     */
    int start( const QString& program,
               const QStringList& arguments,
               const QStringList& environment,
               ulong winid,
               bool addToUtmp
             );

    /**
     * set properties for "EmptyPTY"
     */
    void setEmptyPTYProperties();

    /** TODO: Document me */
    void setWriteable(bool writeable);

    /**
     * Enables or disables Xon/Xoff flow control.  The flow control setting
     * may be changed later by a terminal application, so flowControlEnabled()
     * may not equal the value of @p on in the previous call to setFlowControlEnabled()
     */
    void setFlowControlEnabled(bool on);

    /** Queries the terminal state and returns true if Xon/Xoff flow control is enabled. */
    bool flowControlEnabled() const;

    /**
     * Sets the size of the window (in lines and columns of characters)
     * used by this teletype.
     */
    void setWindowSize(int lines, int cols);

    /** Returns the size of the window used by this teletype.  See setWindowSize() */
    QSize windowSize() const;

    /** TODO Document me */
    void setErase(char erase);

    /** */
    char erase() const;

    /**
     * Returns the process id of the teletype's current foreground
     * process.  This is the process which is currently reading
     * input sent to the terminal via. sendData()
     *
     * If there is a problem reading the foreground process group,
     * 0 will be returned.
     */
    int foregroundProcessGroup() const;

  public slots:

    /**
     * Put the pty into UTF-8 mode on systems which support it.
     */
    void setUtf8Mode(bool on);

    /**
     * Suspend or resume processing of data from the standard
     * output of the terminal process.
     *
     * See K3Process::suspend() and K3Process::resume()
     *
     * @param lock If true, processing of output is suspended,
     * otherwise processing is resumed.
     */
    void lockPty(bool lock);

    /**
     * Sends data to the process currently controlling the
     * teletype ( whose id is returned by foregroundProcessGroup() )
     *
     * @param buffer Pointer to the data to send.
     * @param length Length of @p buffer.
     */
    void sendData(const char* buffer, int length);

  signals:

    /**
     * Emitted when a new block of data is received from
     * the teletype.
     *
     * @param buffer Pointer to the data received.
     * @param length Length of @p buffer
     */
    void receivedData(const char* buffer, int length);

  protected:
      void setupChildProcess();

  private slots:
    // called when data is received from the terminal process
    void dataReceived();

  private:
      void init();

    // takes a list of key=value pairs and adds them
    // to the environment for the process
    void addEnvironmentVariables(const QStringList& environment);

    int  _windowColumns;
    int  _windowLines;
    char _eraseChar;
    bool _xonXoff;
    bool _utf8;
};

#endif // PTY_H
