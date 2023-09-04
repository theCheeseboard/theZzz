#include "endpointprovider.h"

struct EndpointProviderPrivate {
        QString verb = QStringLiteral("GET");
        QString endpoint;
};

EndpointProvider::EndpointProvider() {
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
}

QString EndpointProvider::endpoint() {
    return d->endpoint;
}

void EndpointProvider::setEndpoint(QString endpoint) {
    d->endpoint = endpoint;
}
