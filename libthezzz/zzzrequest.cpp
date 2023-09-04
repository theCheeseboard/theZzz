#include "zzzrequest.h"

struct ZzzRequestPrivate {
        QTreeWidgetItem* treeWidgetItem;
};

ZzzRequest::ZzzRequest(QObject* parent) :
    QObject{parent} {
    d = new ZzzRequestPrivate();
    d->treeWidgetItem = new QTreeWidgetItem();
    d->treeWidgetItem->setText(0, tr("New Request"));
    d->treeWidgetItem->setData(0, Qt::UserRole, QVariant::fromValue(this->sharedFromThis()));
}

ZzzRequest::~ZzzRequest() {
    delete d->treeWidgetItem;
    delete d;
}

QTreeWidgetItem* ZzzRequest::treeWidgetItem() {
    return d->treeWidgetItem;
}
