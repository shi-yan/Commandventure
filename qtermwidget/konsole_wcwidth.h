#ifndef _KONSOLE_WCWIDTH_H_
#define _KONSOLE_WCWIDTH_H_

// Qt
#include <QtGlobal>

class QString;

int konsole_wcwidth(quint16 ucs);
#if 0
int konsole_wcwidth_cjk(Q_UINT16 ucs);
#endif

int string_width( const QString & txt );

#endif
