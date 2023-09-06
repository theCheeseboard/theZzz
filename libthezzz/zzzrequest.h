#ifndef ZZZREQUEST_H
#define ZZZREQUEST_H

#include "forwarddeclares.h"
#include "providers/bodyprovider.h"
#include "providers/endpointprovider.h"
#include "providers/headersprovider.h"
#include "providers/zzzprovides.h"
#include "zzzrequesttreeitem.h"
#include <QObject>

typedef ZzzProvides<HeadersProvider, EndpointProvider, BodyProvider> ZzzRequestZzzProvides;

class QNetworkReply;
struct ZzzRequestPrivate;
class ZzzRequest : public QObject,
                   public ZzzSharedFromThis<ZzzRequest>,
                   public ZzzRequestTreeItem,
                   public ZzzRequestZzzProvides {
        Q_OBJECT
    public:
        explicit ZzzRequest(WorkspaceFilePtr workspace, QObject* parent = nullptr);
        ~ZzzRequest();

        ZzzReplyPtr execute();

    signals:

    private:
        ZzzRequestPrivate* d;

        // ZzzRequestTreeItem interface
    public:
        QTreeWidgetItem* treeWidgetItem();

        // ZzzProvides interface
    public:
        QJsonValue toJson();
};

typedef QSharedPointer<ZzzRequest> ZzzRequestPtr;

#endif // ZZZREQUEST_H
