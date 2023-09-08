#include "requestcontainerprovider.h"

#include "workspacefile.h"
#include "zzzrequest.h"
#include "zzzrequestfolder.h"
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

void RequestContainerProvider::removeRequestRecursive(ZzzRequestTreeItemPtr request) {
    d->requests.removeAll(request);
    for (auto rq : d->requests) {
        if (auto containerProvider = rq.dynamicCast<RequestContainerProvider>()) {
            containerProvider->removeRequestRecursive(request);
        }
    }

    emit this->workspace()->requestsChanged();
}

QList<ZzzRequestTreeItemPtr> RequestContainerProvider::requests() {
    return d->requests;
}

QString RequestContainerProvider::jsonKey() {
    return QStringLiteral("requests");
}

void RequestContainerProvider::loadJson(QJsonValue obj) {
    d->requests.clear();

    auto requests = obj.toObject().value("requests").toArray();
    for (auto request : requests) {
        auto requestObj = request.toObject();
        if (requestObj.value("type") == "request") {
            auto req = (new ZzzRequest(this->workspace()->sharedFromThis().staticCast<WorkspaceFile>()))->sharedFromThis().staticCast<ZzzRequest>();
            req->loadJson(requestObj);
            d->requests.append(req);
        } else if (requestObj.value("type") == "requestfolder") {
            auto req = (new ZzzRequestFolder(this->workspace()->sharedFromThis().staticCast<WorkspaceFile>()))->sharedFromThis().staticCast<ZzzRequestFolder>();
            req->loadJson(requestObj);
            d->requests.append(req);
        }
    }
}

QJsonValue RequestContainerProvider::toJson() {
    QJsonArray requests;
    for (auto request : d->requests) {
        if (auto zrq = request.dynamicCast<ZzzRequest>()) {
            requests.append(zrq->toJson());
        } else if (auto zrqf = request.dynamicCast<ZzzRequestFolder>()) {
            requests.append(zrqf->toJson());
        }
    }

    return QJsonObject{
        {"requests", requests}
    };
}
