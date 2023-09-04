#ifndef ZZZREQUEST_H
#define ZZZREQUEST_H

#include "forwarddeclares.h"
#include "providers/endpointprovider.h"
#include "providers/headersprovider.h"
#include "zzzrequesttreeitem.h"
#include <QObject>

struct ZzzRequestPrivate;
class ZzzRequest : public QObject,
                   public ZzzSharedFromThis<ZzzRequest>,
                   public ZzzRequestTreeItem,
                   public HeadersProvider,
                   public EndpointProvider {
        Q_OBJECT
    public:
        explicit ZzzRequest(QObject* parent = nullptr);
        ~ZzzRequest();

    signals:

    private:
        ZzzRequestPrivate* d;

        // ZzzRequestTreeItem interface
    public:
        QTreeWidgetItem* treeWidgetItem();
};

typedef QSharedPointer<ZzzRequest> ZzzRequestPtr;

#endif // ZZZREQUEST_H
