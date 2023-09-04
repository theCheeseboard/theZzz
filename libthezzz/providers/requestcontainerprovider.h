#ifndef REQUESTCONTAINERPROVIDER_H
#define REQUESTCONTAINERPROVIDER_H

#include "forwarddeclares.h"

struct RequestContainerProviderPrivate;

class WorkspaceFile;
class RequestContainerProvider {
    public:
        RequestContainerProvider(WorkspaceFile* parent);
        ~RequestContainerProvider();

        void addRequest(ZzzRequestTreeItemPtr request);
        QList<ZzzRequestTreeItemPtr> requests();

    private:
        RequestContainerProviderPrivate* d;
};

#endif // REQUESTCONTAINERPROVIDER_H
