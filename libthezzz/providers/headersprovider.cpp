#include "headersprovider.h"

#include "widgets/providereditor/headerprovidereditor.h"
#include "workspacefile.h"
#include <QJsonArray>

struct HeadersProviderPrivate {
        ZzzHeaders headers;
};

HeadersProvider::HeadersProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new HeadersProviderPrivate();
}

HeadersProvider::~HeadersProvider() {
    delete d;
}

ZzzHeaders HeadersProvider::headers() {
    return d->headers;
}

void HeadersProvider::setHeaders(ZzzHeaders headers) {
    d->headers = headers;
    emit this->workspace()->dataChanged();
}

QString HeadersProvider::jsonKey() {
    return QStringLiteral("headers");
}

void HeadersProvider::loadJson(QJsonValue obj) {
    d->headers.clear();

    auto headers = obj.toArray();
    for (auto header : headers) {
        auto headerObj = header.toObject();
        d->headers.append({headerObj.value("header").toString().toUtf8(), headerObj.value("value").toString().toUtf8()});
    }
}

QJsonValue HeadersProvider::toJson() {
    QJsonArray headers;
    for (auto header : d->headers) {
        headers.append(QJsonObject{
            {"header", QString(header.first) },
            {"value",  QString(header.second)}
        });
    }
    return headers;
}

QList<ProviderEditor*> HeadersProvider::editor() {
    return {new HeaderProviderEditor(this)};
}
