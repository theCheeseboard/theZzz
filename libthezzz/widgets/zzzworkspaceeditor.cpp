#include "zzzworkspaceeditor.h"
#include "ui_zzzworkspaceeditor.h"

#include "ranges/trange.h"
#include "zzzrequest.h"
#include "zzzrequesteditor.h"
#include <workspacefile.h>

struct ZzzWorkspaceEditorPrivate {
        WorkspaceFilePtr workspaceFile;
};

ZzzWorkspaceEditor::ZzzWorkspaceEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ZzzWorkspaceEditor) {
    ui->setupUi(this);

    d = new ZzzWorkspaceEditorPrivate();
    d->workspaceFile = (new WorkspaceFile())->sharedFromThis();

    connect(d->workspaceFile.data(), &WorkspaceFile::requestsChanged, this, [this] {
        this->updateRequests(ui->treeWidget->invisibleRootItem(), d->workspaceFile.dynamicCast<RequestContainerProvider>());
    });
    this->updateRequests(ui->treeWidget->invisibleRootItem(), d->workspaceFile.dynamicCast<RequestContainerProvider>());

    ui->leftWidget->setFixedWidth(400);
}

ZzzWorkspaceEditor::~ZzzWorkspaceEditor() {
    delete d;
    delete ui;
}

void ZzzWorkspaceEditor::on_newRequestButton_clicked() {
    d->workspaceFile->addRequest((new ZzzRequest())->sharedFromThis());
}

void ZzzWorkspaceEditor::updateRequests(QTreeWidgetItem* rootItem, RequestContainerProviderPtr containerProvider) {
    auto items = tRange(containerProvider->requests()).map<QTreeWidgetItem*>([](ZzzRequestTreeItemPtr treeItem) {
                                                          return treeItem->treeWidgetItem();
                                                      })
                     .toList();
    for (auto i = 0; i < items.length(); i++) {
        auto item = items.at(i);
        if (item->childCount() > i) {
            auto child = item->child(i);
            if (item == child) continue; // Nothing to change

            rootItem->removeChild(item);
            i -= 1;
            continue;
        }

        rootItem->addChild(item);
    }
}

void ZzzWorkspaceEditor::on_treeWidget_itemSelectionChanged() {
    auto items = ui->treeWidget->selectedItems();
    if (items.length() == 0) {
    } else if (items.length() == 1) {
        auto request = items.constFirst()->data(0, Qt::UserRole).value<ZzzRequestPtr>();
        if (!request) return;

        auto editor = new ZzzRequestEditor(request);
        ui->stackedWidget->addWidget(editor);
        ui->stackedWidget->setCurrentWidget(editor);
    } else {
    }
}
