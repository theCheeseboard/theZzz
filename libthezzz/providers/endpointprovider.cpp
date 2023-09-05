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

QString EndpointProvider::jsonKey() {
    return QStringLiteral("endpoint");
}

void EndpointProvider::loadJson(QJsonObject obj) {
    d->verb = obj.value("verb").toString();
    d->endpoint = obj.value("endpoint").toString();
}

QJsonObject EndpointProvider::toJson() {
    return {
        {"verb",     d->verb    },
        {"endpoint", d->endpoint}
    };
}
