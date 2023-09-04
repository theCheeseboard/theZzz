#ifndef ZZZREQUESTTREEITEM_H
#define ZZZREQUESTTREEITEM_H

#include <QSharedPointer>
#include <QTreeWidgetItem>

class ZzzRequestTreeItem {
    public:
        ZzzRequestTreeItem();

        virtual QTreeWidgetItem* treeWidgetItem() = 0;
};

typedef QSharedPointer<ZzzRequestTreeItem> RequestTreeItemPtr;

#endif // ZZZREQUESTTREEITEM_H
