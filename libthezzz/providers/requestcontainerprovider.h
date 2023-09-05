#ifndef REQUESTCONTAINERPROVIDER_H
#define REQUESTCONTAINERPROVIDER_H

#include "forwarddeclares.h"
#include "zzzprovider.h"

struct RequestContainerProviderPrivate;

class WorkspaceFile;
class RequestContainerProvider : public ZzzProvider<RequestContainerProvider> {
    public:
        RequestContainerProvider(WorkspaceFile* parent);
        ~RequestContainerProvider();

        void addRequest(ZzzRequestTreeItemPtr request);
        QList<ZzzRequestTreeItemPtr> requests();

    private:
        RequestContainerProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonObject obj);
        QJsonObject toJson();
};

#endif // REQUESTCONTAINERPROVIDER_H
