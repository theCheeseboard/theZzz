#include "requestcontainerprovider.h"

#include "workspacefile.h"
#include <QList>

struct RequestContainerProviderPrivate {
        WorkspaceFile* parent;
        QList<ZzzRequestTreeItemPtr> requests;
};

RequestContainerProvider::RequestContainerProvider(WorkspaceFile* parent) {
    d = new RequestContainerProviderPrivate();
    d->parent = parent;
}

RequestContainerProvider::~RequestContainerProvider() {
    delete d;
}

void RequestContainerProvider::addRequest(ZzzRequestTreeItemPtr request) {
    d->requests.append(request);
    emit d->parent->requestsChanged();
}

QList<ZzzRequestTreeItemPtr> RequestContainerProvider::requests() {
    return d->requests;
}
