#ifndef kpty_p_h
#define kpty_p_h

#include "kpty.h"

#include <QByteArray>

class KPtyPrivate {
public:

    Q_DECLARE_PUBLIC(KPty)

    KPtyPrivate(KPty* parent);
    virtual ~KPtyPrivate();

#ifndef HAVE_OPENPTY
    bool chownpty(bool grant);
#endif

    int masterFd;
    int slaveFd;
    bool ownMaster:1;

    QByteArray ttyName;

    KPty *q_ptr;
};

#endif
