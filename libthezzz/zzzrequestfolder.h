#ifndef ZZZREQUESTFOLDER_H
#define ZZZREQUESTFOLDER_H

#include "providers/headersprovider.h"
#include "providers/requestcontainerprovider.h"
#include "zzzrequesttreeitem.h"
#include <QObject>

typedef ZzzProvides<HeadersProvider, RequestContainerProvider> ZzzRequestFolderZzzProvides;

struct ZzzRequestFolderPrivate;
class ZzzRequestFolder : public QObject,
                         public ZzzRequestTreeItem,
                         public ZzzRequestFolderZzzProvides {
        Q_OBJECT
    public:
        explicit ZzzRequestFolder(WorkspaceFilePtr workspace, QObject* parent = nullptr);
        ~ZzzRequestFolder();

    signals:

    private:
        ZzzRequestFolderPrivate* d;

        // ZzzRequestTreeItem interface
    public:
        QTreeWidgetItem* treeWidgetItem();
        QJsonValue toJson();
};

#endif // ZZZREQUESTFOLDER_H
