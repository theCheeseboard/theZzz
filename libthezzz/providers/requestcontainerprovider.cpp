#include "requestcontainerprovider.h"

#include "workspacefile.h"
#include "zzzrequest.h"
#include <QJsonArray>
#include <QList>

struct RequestContainerProviderPrivate {
        QList<ZzzRequestTreeItemPtr> requests;
};

RequestContainerProvider::RequestContainerProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new RequestContainerProviderPrivate();
}

RequestContainerProvider::~RequestContainerProvider() {
    delete d;
}

void RequestContainerProvider::addRequest(ZzzRequestTreeItemPtr request) {
    d->requests.append(request);
    emit this->workspace()->requestsChanged();
}

QList<ZzzRequestTreeItemPtr> RequestContainerProvider::requests() {
    return d->requests;
}

QString RequestContainerProvider::jsonKey() {
    return QStringLiteral("requests");
}

void RequestContainerProvider::loadJson(QJsonObject obj) {
    d->requests.clear();

    auto requests = obj.value("requests").toArray();
    for (auto request : requests) {
        auto requestObj = request.toObject();
        if (requestObj.value("type") == "request") {
            auto req = (new ZzzRequest(this->workspace()->sharedFromThis()))->sharedFromThis();
            req->loadJson(requestObj);
            d->requests.append(req);
        }
    }
}

QJsonObject RequestContainerProvider::toJson() {
    QJsonArray requests;
    for (auto request : d->requests) {
        if (auto zrq = request.dynamicCast<ZzzRequest>()) {
            requests.append(zrq->toJson());
        }
    }

    return {
        {"requests", requests}
    };
}
