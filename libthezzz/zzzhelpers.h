#ifndef ZZZHELPERS_H
#define ZZZHELPERS_H

#include <QIcon>
#include "libthezzz_global.h"

typedef QList<std::pair<QByteArray, QByteArray>> ZzzHeaders;

namespace ZzzHelpers {
    LIBTHEZZZ_EXPORT QIcon iconForVerb(QString verb);
};

#endif // ZZZHELPERS_H
