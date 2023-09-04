#include "headersprovider.h"

struct HeadersProviderPrivate {
};

HeadersProvider::HeadersProvider() {
    d = new HeadersProviderPrivate();
}

HeadersProvider::~HeadersProvider() {
    delete d;
}

void HeadersProvider::loadHeadersJson(QJsonObject obj) {
}

QJsonObject HeadersProvider::headersToJson() {
    return {};
}
