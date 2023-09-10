#include "endpointprovider.h"

#include "workspacefile.h"

struct EndpointProviderPrivate {
        QString verb = QStringLiteral("GET");
        QString endpoint;
};

EndpointProvider::EndpointProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new EndpointProviderPrivate();
}

EndpointProvider::~EndpointProvider() {
    delete d;
}

QString EndpointProvider::verb() {
    return d->verb;
}

void EndpointProvider::setVerb(QString verb) {
    d->verb = verb;
    emit this->workspace()->dataChanged();
}

QString EndpointProvider::endpoint() {
    return d->endpoint;
}

void EndpointProvider::setEndpoint(QString endpoint) {
    d->endpoint = endpoint;
    emit this->workspace()->dataChanged();
}

QUrl EndpointProvider::calculateUrl() {
    return d->endpoint;
}

QString EndpointProvider::jsonKey() {
    return QStringLiteral("endpoint");
}

void EndpointProvider::loadJson(QJsonValue obj, QJsonValue localObj) {
    auto object = obj.toObject();
    d->verb = object.value("verb").toString();
    d->endpoint = object.value("endpoint").toString();
}

QJsonValue EndpointProvider::toJson() {
    return QJsonObject{
        {"verb",     d->verb    },
        {"endpoint", d->endpoint}
    };
}

ZzzHeaders EndpointProvider::implicitHeaders() {
    return {
        {QStringLiteral("Host").toUtf8(), this->calculateUrl().host().toUtf8()},
    };
}
