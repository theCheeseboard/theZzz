#include "workspacefile.h"

#include "zzznetworkcache.h"
#include "zzzrequest.h"
#include <QAbstractNetworkCache>
#include <QJsonObject>
#include <QNetworkAccessManager>

struct WorkspaceFilePrivate {
        QNetworkAccessManager networkAccessManager;
        QTreeWidgetItem* treeWidgetItem;
};

WorkspaceFile::WorkspaceFile(QObject* parent) :
    QObject{parent}, WorkspaceFileZzzProvides(this) {
    d = new WorkspaceFilePrivate();
    d->networkAccessManager.setCache(new ZzzNetworkCache(this));

    Q_INIT_RESOURCE(libthezzz_icons);

    d->treeWidgetItem = new QTreeWidgetItem();
    d->treeWidgetItem->setText(0, workspaceFileTitle());
    d->treeWidgetItem->setData(0, Qt::UserRole, QVariant::fromValue(this->ZzzRequestTreeItem::sharedFromThis()));
    connect(this, &WorkspaceFile::dataChanged, this, [this] {
        d->treeWidgetItem->setText(0, workspaceFileTitle());
    });

    this->addRequest((new ZzzRequest(this->sharedFromThis().staticCast<WorkspaceFile>()))->sharedFromThis());
}

WorkspaceFile::~WorkspaceFile() {
    delete d;
}

void WorkspaceFile::loadJson(QJsonValue object) {
    WorkspaceFileZzzProvides::loadJson(object.toObject());
    emit requestsChanged();
    emit dataChanged();
}

QJsonValue WorkspaceFile::toJson() {
    return WorkspaceFileZzzProvides::toJson();
}

QString WorkspaceFile::workspaceFileTitle() {
    return this->title().isEmpty() ? tr("Untitled Workspace") : this->title();
}

QNetworkAccessManager* WorkspaceFile::networkAccessManager() {
    return &d->networkAccessManager;
}

QTreeWidgetItem* WorkspaceFile::treeWidgetItem() {
    return d->treeWidgetItem;
}
