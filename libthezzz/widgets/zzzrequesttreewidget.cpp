#include "zzzrequesttreewidget.h"

#include "forwarddeclares.h"
#include "providers/requestcontainerprovider.h"
#include "workspacefile.h"
#include <QDropEvent>

struct ZzzRequestTreeWidgetPrivate {
        QTreeWidgetItem* draggedItem;
};

ZzzRequestTreeWidget::ZzzRequestTreeWidget(QWidget* parent) {
    d = new ZzzRequestTreeWidgetPrivate();
}

ZzzRequestTreeWidget::~ZzzRequestTreeWidget() {
    delete d;
}

void ZzzRequestTreeWidget::dragEnterEvent(QDragEnterEvent* event) {
    d->draggedItem = this->currentItem();
    QTreeWidget::dragEnterEvent(event);
}

void ZzzRequestTreeWidget::dropEvent(QDropEvent* event) {
    auto droppedOnto = this->itemAt(event->position().toPoint());
    if (!droppedOnto || !d->draggedItem || !droppedOnto->parent()) {
        event->ignore();
        return;
    }

    auto item = d->draggedItem->data(0, Qt::UserRole).value<ZzzRequestTreeItemPtr>();
    auto dropped = droppedOnto->data(0, Qt::UserRole).value<ZzzRequestTreeItemPtr>();
    auto newParent = droppedOnto->parent()->data(0, Qt::UserRole).value<ZzzRequestTreeItemPtr>();
    auto parentContainer = newParent.dynamicCast<RequestContainerProvider>();
    auto workspace = parentContainer->workspace();

    workspace->removeRequestRecursive(item);
    parentContainer->insertRequest(dropped, item);

    event->ignore();
}
