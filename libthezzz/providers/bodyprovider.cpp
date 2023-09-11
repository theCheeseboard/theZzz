#include "bodyprovider.h"

#include "widgets/providereditor/bodyprovidereditor.h"
#include "workspacefile.h"
#include <QJsonObject>

struct BodyProviderPrivate {
        QByteArray body;
        QString contentType;
};

BodyProvider::BodyProvider(WorkspaceFile* parent) :
    ZzzProvider{parent} {
    d = new BodyProviderPrivate();
    d->contentType = "text/plain";
}

BodyProvider::~BodyProvider() {
    delete d;
}

QByteArray BodyProvider::body() {
    return d->body;
}

void BodyProvider::setBody(QByteArray body) {
    d->body = body;
    emit this->workspace()->dataChanged();
}

QString BodyProvider::contentType() {
    return d->contentType;
}

void BodyProvider::setContentType(QString contentType) {
    d->contentType = contentType;
    emit this->workspace()->dataChanged();
}

QString BodyProvider::jsonKey() {
    return QStringLiteral("body");
}

void BodyProvider::loadJson(QJsonValue obj, QJsonValue localObj) {
    auto object = obj.toObject();
    d->body = QByteArray::fromBase64(object.value("payload").toString().toUtf8());
    d->contentType = object.value("contentType").toString("text/plain");
}

QJsonValue BodyProvider::toJson() {
    return QJsonObject{
        {"payload",     QString(d->body.toBase64())},
        {"contentType", d->contentType             }
    };
}

QList<ProviderEditor*> BodyProvider::editor() {
    return {new BodyProviderEditor(this)};
}

ZzzHeaders BodyProvider::implicitHeaders() {
    return {
        {QStringLiteral("Content-Length").toUtf8(), QStringLiteral("%1").arg(d->body.length()).toUtf8()},
        {QStringLiteral("Content-Type").toUtf8(),   d->contentType.toUtf8()                            }
    };
}
