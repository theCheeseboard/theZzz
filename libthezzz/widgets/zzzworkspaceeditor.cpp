#include "zzzworkspaceeditor.h"
#include "ui_zzzworkspaceeditor.h"

#include "ranges/trange.h"
#include "zzzrequest.h"
#include "zzzrequesteditor.h"
#include <QFile>
#include <QJsonDocument>
#include <workspacefile.h>

struct ZzzWorkspaceEditorPrivate {
        WorkspaceFilePtr workspaceFile;
        QTreeWidgetItem* workspaceItem;

        QString filePath;
};

ZzzWorkspaceEditor::ZzzWorkspaceEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ZzzWorkspaceEditor) {
    ui->setupUi(this);

    d = new ZzzWorkspaceEditorPrivate();
    d->workspaceFile = (new WorkspaceFile())->sharedFromThis();
    d->workspaceItem = new QTreeWidgetItem();
    d->workspaceItem->setText(0, tr("Workspace"));
    d->workspaceItem->setData(0, Qt::UserRole, QVariant::fromValue(d->workspaceFile));

    ui->treeWidget->addTopLevelItem(d->workspaceItem);

    connect(d->workspaceFile.data(), &WorkspaceFile::requestsChanged, this, [this] {
        this->updateRequests(d->workspaceItem, d->workspaceFile.dynamicCast<RequestContainerProvider>());
    });
    connect(d->workspaceFile.data(), &WorkspaceFile::dataChanged, this, [this] {
        if (!d->filePath.isEmpty()) {
            this->saveWorkspace(d->filePath);
        }
    });
    this->updateRequests(d->workspaceItem, d->workspaceFile.dynamicCast<RequestContainerProvider>());

    ui->leftWidget->setFixedWidth(300);
}

ZzzWorkspaceEditor::~ZzzWorkspaceEditor() {
    delete d;
    delete ui;
}

QString ZzzWorkspaceEditor::currentFile() {
    return d->filePath;
}

void ZzzWorkspaceEditor::saveWorkspace(QString filePath) {
    auto data = QJsonDocument(d->workspaceFile->toJson()).toJson();

    QFile file(filePath);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();

    d->filePath = filePath;
}

void ZzzWorkspaceEditor::openWorkspace(QString filePath) {
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    auto data = file.readAll();
    file.close();

    QJsonParseError error;
    auto json = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        // TODO: Show an error
        return;
    }

    d->workspaceFile->loadJson(json.object());
    d->filePath = filePath;
}

void ZzzWorkspaceEditor::on_newRequestButton_clicked() {
    d->workspaceFile->addRequest((new ZzzRequest(d->workspaceFile))->sharedFromThis());
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

    d->workspaceItem->setExpanded(true);
}

void ZzzWorkspaceEditor::on_treeWidget_itemSelectionChanged() {
    auto items = ui->treeWidget->selectedItems();
    if (items.length() == 0) {
    } else if (items.length() == 1) {
        QSharedPointer<QObject> request = items.constFirst()->data(0, Qt::UserRole).value<ZzzRequestPtr>();
        if (!request) {
            request = items.constFirst()->data(0, Qt::UserRole).value<WorkspaceFilePtr>();
            if (!request) return;
        }

        auto editor = new ZzzRequestEditor(request);
        connect(editor, &ZzzRequestEditor::addReply, this, &ZzzWorkspaceEditor::addReply);
        ui->stackedWidget->addWidget(editor);
        ui->stackedWidget->setCurrentWidget(editor);
    } else {
    }
}
