#include "workspacefile.h"

#include "zzzrequest.h"
#include <QJsonObject>
#include <QNetworkAccessManager>

struct WorkspaceFilePrivate {
        QNetworkAccessManager networkAccessManager;
};

WorkspaceFile::WorkspaceFile(QObject* parent) :
    QObject{parent}, RequestContainerProvider{this} {
    d = new WorkspaceFilePrivate();

    Q_INIT_RESOURCE(libthezzz_icons);

    this->addRequest((new ZzzRequest(this->sharedFromThis()))->sharedFromThis());
}

WorkspaceFile::~WorkspaceFile() {
    delete d;
}

void WorkspaceFile::loadJson(QJsonObject object) {
}

QJsonObject WorkspaceFile::toJson() {
    QJsonObject obj;
    return obj;
}

QNetworkAccessManager* WorkspaceFile::networkAccessManager() {
    return &d->networkAccessManager;
}
