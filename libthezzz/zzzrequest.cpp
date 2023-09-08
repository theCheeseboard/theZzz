#include "zzzrequest.h"

#include "workspacefile.h"
#include "zzzreply.h"
#include <QNetworkAccessManager>

struct ZzzRequestPrivate {
        WorkspaceFilePtr workspace;
        QTreeWidgetItem* treeWidgetItem;
};

ZzzRequest::ZzzRequest(WorkspaceFilePtr workspace, QObject* parent) :
    QObject{parent}, ZzzRequestZzzProvides(workspace.data()) {
    d = new ZzzRequestPrivate();
    d->treeWidgetItem = new QTreeWidgetItem();
    d->treeWidgetItem->setText(0, this->title().isEmpty() ? tr("Untitled Request") : this->title());
    d->treeWidgetItem->setData(0, Qt::UserRole, QVariant::fromValue(this->ZzzRequestTreeItem::sharedFromThis()));

    connect(workspace.data(), &WorkspaceFile::dataChanged, this, [this] {
        d->treeWidgetItem->setText(0, this->title().isEmpty() ? tr("Untitled Request") : this->title());
    });
    d->workspace = workspace;
}

ZzzRequest::~ZzzRequest() {
    delete d->treeWidgetItem;
    delete d;
}

ZzzReplyPtr ZzzRequest::execute() {
    auto mgr = d->workspace->networkAccessManager();

    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setUrl(this->endpoint());

    for (const auto& header : this->implicitHeadersWithAncestors()) {
        request.setRawHeader(header.first, header.second);
    }

    for (const auto& header : this->ancestorHeaders()) {
        request.setRawHeader(header.first, header.second);
    }

    // TODO: Concatenate multiple headers correctly
    for (const auto& header : this->headers()) {
        request.setRawHeader(header.first, header.second);
    }

    QNetworkReply* networkReply;
    if (this->verb() == "GET") {
        networkReply = mgr->get(request);
    } else if (this->verb() == "POST") {
        networkReply = mgr->post(request, this->body());
    } else if (this->verb() == "PUT") {
        networkReply = mgr->put(request, this->body());
    } else if (this->verb() == "DELETE") {
        networkReply = mgr->deleteResource(request);
    } else {
        networkReply = mgr->sendCustomRequest(request, this->verb().toUtf8(), this->body());
    }

    auto reply = new ZzzReply(this->verb(), request, networkReply);
    return reply->sharedFromThis();
}

QTreeWidgetItem* ZzzRequest::treeWidgetItem() {
    return d->treeWidgetItem;
}

QJsonValue ZzzRequest::toJson() {
    auto json = ZzzRequestZzzProvides::toJson().toObject();
    json.insert("type", "request");
    return json;
}
