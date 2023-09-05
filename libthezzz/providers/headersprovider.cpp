#include "headersprovider.h"

struct HeadersProviderPrivate {
};

HeadersProvider::HeadersProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new HeadersProviderPrivate();
}

HeadersProvider::~HeadersProvider() {
    delete d;
}

QString HeadersProvider::jsonKey() {
    return QStringLiteral("headers");
}

void HeadersProvider::loadJson(QJsonObject obj) {
}

QJsonObject HeadersProvider::toJson() {
    return {};
}
