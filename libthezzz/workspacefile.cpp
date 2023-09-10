#include "workspacefile.h"

#include "zzznetworkcache.h"
#include "zzzrequest.h"
#include <QAbstractNetworkCache>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QStandardPaths>

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

QString WorkspaceFile::localJsonFilePath(QUuid workspaceUuid) {
    auto appData = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).absoluteFilePath("workspaces");
    return QDir(appData).absoluteFilePath(workspaceUuid.toString(QUuid::WithoutBraces));
}

void WorkspaceFile::loadJson(QJsonValue object) {
    QJsonValue localObj;
    auto objectIdentifier = object.toObject().value("identifier").toString();
    if (!objectIdentifier.isEmpty()) {
        auto uuid = QUuid::fromString(objectIdentifier);
        if (!uuid.isNull()) {
            auto localJsonFile = localJsonFilePath(uuid);

            QFile localJson(localJsonFile);
            localJson.open(QFile::ReadOnly);
            localObj = QJsonDocument::fromJson(localJson.readAll()).object();
            localJson.close();
        }
    }

    this->loadJson(object, localObj);
}

void WorkspaceFile::loadJson(QJsonValue object, QJsonValue localObj) {
    WorkspaceFileZzzProvides::loadJson(object.toObject(), localObj.toObject());
    emit requestsChanged();
    emit dataChanged();
}

QJsonValue WorkspaceFile::toJson() {
    if (!this->identifier().isNull()) {
        auto localJsonFile = localJsonFilePath(this->identifier());

        QDir::root().mkpath(QFileInfo(localJsonFile).dir().path());

        QFile localJson(localJsonFile);
        localJson.open(QFile::WriteOnly);
        localJson.write(QJsonDocument(this->toLocalJson().toObject()).toJson());
        localJson.close();
    }

    return WorkspaceFileZzzProvides::toJson();
}

QJsonValue WorkspaceFile::toLocalJson() {
    return WorkspaceFileZzzProvides::toLocalJson();
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
