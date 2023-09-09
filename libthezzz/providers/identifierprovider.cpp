#include "identifierprovider.h"

struct IdentifierProviderPrivate {
        QUuid identifier;
};

IdentifierProvider::IdentifierProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new IdentifierProviderPrivate();
    d->identifier = QUuid::createUuid();
}

IdentifierProvider::~IdentifierProvider() {
    delete d;
}

QUuid IdentifierProvider::identifier() {
    return d->identifier;
}

QString IdentifierProvider::jsonKey() {
    return QStringLiteral("identifier");
}

void IdentifierProvider::loadJson(QJsonValue obj) {
    if (obj.isString()) {
        d->identifier = QUuid::fromString(obj.toString());
    }
}

QJsonValue IdentifierProvider::toJson() {
    return d->identifier.toString(QUuid::WithoutBraces);
}
