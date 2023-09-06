#include "workspacefile.h"

#include "zzznetworkcache.h"
#include "zzzrequest.h"
#include <QAbstractNetworkCache>
#include <QJsonObject>
#include <QNetworkAccessManager>

struct WorkspaceFilePrivate {
        QNetworkAccessManager networkAccessManager;
};

WorkspaceFile::WorkspaceFile(QObject* parent) :
    QObject{parent}, WorkspaceFileZzzProvides(this) {
    d = new WorkspaceFilePrivate();
    d->networkAccessManager.setCache(new ZzzNetworkCache(this));

    Q_INIT_RESOURCE(libthezzz_icons);

    this->addRequest((new ZzzRequest(this->sharedFromThis()))->sharedFromThis());
}

WorkspaceFile::~WorkspaceFile() {
    delete d;
}

void WorkspaceFile::loadJson(QJsonValue object) {
    WorkspaceFileZzzProvides::loadJson(object.toObject());
    emit requestsChanged();
}

QJsonValue WorkspaceFile::toJson() {
    return WorkspaceFileZzzProvides::toJson();
}

QNetworkAccessManager* WorkspaceFile::networkAccessManager() {
    return &d->networkAccessManager;
}
