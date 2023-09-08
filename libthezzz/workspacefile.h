#ifndef WORKSPACEFILE_H
#define WORKSPACEFILE_H

#include "forwarddeclares.h"
#include "libthezzz_global.h"
#include "providers/descriptionprovider.h"
#include "providers/headersprovider.h"
#include "providers/requestcontainerprovider.h"
#include "providers/zzzprovides.h"
#include <QObject>

typedef ZzzProvides<HeadersProvider, RequestContainerProvider, DescriptionProvider> WorkspaceFileZzzProvides;

class QNetworkAccessManager;
struct WorkspaceFilePrivate;
class LIBTHEZZZ_EXPORT WorkspaceFile : public QObject,
                                       public ZzzRequestTreeItem,
                                       public WorkspaceFileZzzProvides {
        Q_OBJECT
    public:
        explicit WorkspaceFile(QObject* parent = nullptr);
        ~WorkspaceFile();

        void loadJson(QJsonValue object);
        QJsonValue toJson();

        QString workspaceFileTitle();

        QNetworkAccessManager* networkAccessManager();

    signals:
        void requestsChanged();
        void dataChanged();

    private:
        WorkspaceFilePrivate* d;

        // ZzzRequestTreeItem interface
    public:
        QTreeWidgetItem* treeWidgetItem();
};

typedef QSharedPointer<WorkspaceFile> WorkspaceFilePtr;

#endif // WORKSPACEFILE_H
