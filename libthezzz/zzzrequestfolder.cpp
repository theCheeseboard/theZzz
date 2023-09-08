#include "zzzrequestfolder.h"

struct ZzzRequestFolderPrivate {
        WorkspaceFilePtr workspace;
        QTreeWidgetItem* treeWidgetItem;
};

ZzzRequestFolder::ZzzRequestFolder(WorkspaceFilePtr workspace, QObject* parent) :
    QObject{parent}, ZzzRequestFolderZzzProvides(workspace.data()) {
    d = new ZzzRequestFolderPrivate();
    d->treeWidgetItem = new QTreeWidgetItem();
    d->treeWidgetItem->setText(0, tr("New Request Folder"));
    d->treeWidgetItem->setData(0, Qt::UserRole, QVariant::fromValue(this->ZzzRequestTreeItem::sharedFromThis()));
}

ZzzRequestFolder::~ZzzRequestFolder() {
    delete d;
}

QTreeWidgetItem* ZzzRequestFolder::treeWidgetItem() {
    return d->treeWidgetItem;
}

QJsonValue ZzzRequestFolder::toJson() {
    auto json = ZzzRequestFolderZzzProvides::toJson().toObject();
    json.insert("type", "requestfolder");
    return json;
}
