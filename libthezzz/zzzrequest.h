#ifndef ZZZREQUEST_H
#define ZZZREQUEST_H

#include "forwarddeclares.h"
#include "libthezzz_global.h"
#include "providers/bodyprovider.h"
#include "providers/descriptionprovider.h"
#include "providers/endpointprovider.h"
#include "providers/headersprovider.h"
#include "providers/zzzprovides.h"
#include "zzzrequesttreeitem.h"
#include <QObject>

typedef ZzzProvides<HeadersProvider, EndpointProvider, BodyProvider, DescriptionProvider> ZzzRequestZzzProvides;

class QNetworkReply;
struct ZzzRequestPrivate;
class LIBTHEZZZ_EXPORT ZzzRequest : public QObject,
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

class LIBTHEZZZ_EXPORT EnvironmentIncompleteException : public tException {
        T_EXCEPTION(EnvironmentIncompleteException)
    public:
        EnvironmentIncompleteException(QList<ZzzVariable> missingVariables);

        QList<ZzzVariable> missingVariables();

    private:
        QList<ZzzVariable> _missingVariables;
};

#endif // ZZZREQUEST_H
