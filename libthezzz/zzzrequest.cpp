#include "zzzrequest.h"

#include "workspacefile.h"
#include "zzzreply.h"
#include <QNetworkAccessManager>
#include <ranges/trange.h>

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
    QList<ZzzVariable> missingEnvironmentVariables;
    auto mgr = d->workspace->networkAccessManager();

    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setUrl(this->calculateUrl(&missingEnvironmentVariables));

    for (const auto& header : this->implicitHeadersWithAncestors()) {
        request.setRawHeader(header.first, d->workspace->substituteEnvironment(header.second, &missingEnvironmentVariables).toUtf8());
    }

    for (const auto& header : this->ancestorHeaders()) {
        request.setRawHeader(header.first, d->workspace->substituteEnvironment(header.second, &missingEnvironmentVariables).toUtf8());
    }

    // TODO: Concatenate multiple headers correctly
    for (const auto& header : this->headers()) {
        request.setRawHeader(header.first, d->workspace->substituteEnvironment(header.second, &missingEnvironmentVariables).toUtf8());
    }

    missingEnvironmentVariables = tRange(missingEnvironmentVariables).unique<QUuid>([](ZzzVariable variable) {
                                                                         return std::get<0>(variable);
                                                                     })
                                      .toList();
    if (!missingEnvironmentVariables.isEmpty()) {
        throw EnvironmentIncompleteException(missingEnvironmentVariables);
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

    bool skipTlsVerification = this->skipTlsVerification();
    connect(networkReply, &QNetworkReply::sslErrors, [skipTlsVerification, networkReply] {
        if (skipTlsVerification) {
            networkReply->ignoreSslErrors();
        }
    });

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

EnvironmentIncompleteException::EnvironmentIncompleteException(QList<ZzzVariable> missingVariables) {
    _missingVariables = missingVariables;
}

QList<ZzzVariable> EnvironmentIncompleteException::missingVariables() {
    return _missingVariables;
}

T_EXCEPTION_IMPL(EnvironmentIncompleteException)
