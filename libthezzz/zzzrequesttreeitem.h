#ifndef ZZZREQUESTTREEITEM_H
#define ZZZREQUESTTREEITEM_H

#include "libthezzz_global.h"
#include "providers/zzzprovides.h"
#include <QSharedPointer>
#include <QTreeWidgetItem>

class LIBTHEZZZ_EXPORT ZzzRequestTreeItem : public ZzzSharedFromThis<ZzzRequestTreeItem> {
    public:
        ZzzRequestTreeItem();
        virtual ~ZzzRequestTreeItem() = default;

        virtual QTreeWidgetItem* treeWidgetItem() = 0;
};

typedef QSharedPointer<ZzzRequestTreeItem> RequestTreeItemPtr;

#endif // ZZZREQUESTTREEITEM_H
