#include "zzzrequest.h"

#include "workspacefile.h"
#include "zzzreply.h"
#include <QNetworkAccessManager>

struct ZzzRequestPrivate {
        WorkspaceFilePtr workspace;
        QTreeWidgetItem* treeWidgetItem;
};

ZzzRequest::ZzzRequest(WorkspaceFilePtr workspace, QObject* parent) :
    QObject{parent} {
    d = new ZzzRequestPrivate();
    d->treeWidgetItem = new QTreeWidgetItem();
    d->treeWidgetItem->setText(0, tr("New Request"));
    d->treeWidgetItem->setData(0, Qt::UserRole, QVariant::fromValue(this->sharedFromThis()));

    d->workspace = workspace;
}

ZzzRequest::~ZzzRequest() {
    delete d->treeWidgetItem;
    delete d;
}

ZzzReplyPtr ZzzRequest::execute() {
    auto mgr = d->workspace->networkAccessManager();

    QNetworkRequest request;
    request.setUrl(this->endpoint());

    QNetworkReply* networkReply;
    if (this->verb() == "GET") {
        networkReply = mgr->get(request);
    } else if (this->verb() == "POST") {
        networkReply = mgr->post(request, QByteArray());
    } else if (this->verb() == "PUT") {
        networkReply = mgr->put(request, QByteArray());
    } else if (this->verb() == "DELETE") {
        networkReply = mgr->deleteResource(request);
    } else {
        networkReply = mgr->sendCustomRequest(request, this->verb().toUtf8(), QByteArray());
    }

    auto reply = new ZzzReply(this->verb(), request, networkReply);
    return reply->sharedFromThis();
}

QTreeWidgetItem* ZzzRequest::treeWidgetItem() {
    return d->treeWidgetItem;
}
