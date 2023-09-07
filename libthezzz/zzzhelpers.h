#ifndef ZZZHELPERS_H
#define ZZZHELPERS_H

#include "libthezzz_global.h"
#include <QIcon>

typedef std::pair<QByteArray, QByteArray> ZzzHeader;
typedef QList<ZzzHeader> ZzzHeaders;

namespace ZzzHelpers {
    LIBTHEZZZ_EXPORT QIcon iconForVerb(QString verb);
};

#endif // ZZZHELPERS_H
