#ifndef ZZZHELPERS_H
#define ZZZHELPERS_H

#include <QIcon>

typedef QList<std::pair<QByteArray, QByteArray>> ZzzHeaders;

namespace ZzzHelpers {
    QIcon iconForVerb(QString verb);
};

#endif // ZZZHELPERS_H
