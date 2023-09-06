#include "zzzhelpers.h"

QIcon ZzzHelpers::iconForVerb(QString verb) {
    if (verb == QStringLiteral("GET")) {
        return QIcon(":/libthezzz/icons/request-get.svg");
    } else if (verb == QStringLiteral("POST")) {
        return QIcon(":/libthezzz/icons/request-post.svg");
    } else if (verb == QStringLiteral("PUT")) {
        return QIcon(":/libthezzz/icons/request-put.svg");
    } else if (verb == QStringLiteral("PATCH")) {
        return QIcon(":/libthezzz/icons/request-patch.svg");
    } else if (verb == QStringLiteral("OPTIONS")) {
        return QIcon(":/libthezzz/icons/request-options.svg");
    } else if (verb == QStringLiteral("HEAD")) {
        return QIcon(":/libthezzz/icons/request-head.svg");
    } else if (verb == QStringLiteral("DELETE")) {
        return QIcon(":/libthezzz/icons/request-delete.svg");
    } else {
        return QIcon();
    }
}
