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

void HeadersProvider::loadJson(QJsonValue obj) {
}

QJsonValue HeadersProvider::toJson() {
    return QJsonObject{};
}
