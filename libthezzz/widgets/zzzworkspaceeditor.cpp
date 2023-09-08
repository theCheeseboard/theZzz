#include "zzzworkspaceeditor.h"
#include "ui_zzzworkspaceeditor.h"

#include "ranges/trange.h"
#include "zzzhelpers.h"
#include "zzzrequest.h"
#include "zzzrequesteditor.h"
#include <QCryptographicHash>
#include <QFile>
#include <QFileSystemWatcher>
#include <QJsonDocument>
#include <QMenu>
#include <QPainter>
#include <workspacefile.h>

struct ZzzWorkspaceEditorPrivate {
        WorkspaceFilePtr workspaceFile;
        QFileSystemWatcher filesystemWatcher;

        QString filePath;
        QByteArray fileHash;
};

ZzzWorkspaceEditor::ZzzWorkspaceEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ZzzWorkspaceEditor) {
    ui->setupUi(this);

    d = new ZzzWorkspaceEditorPrivate();
    d->workspaceFile = (new WorkspaceFile())->sharedFromThis().staticCast<WorkspaceFile>();
    ui->treeWidget->addTopLevelItem(d->workspaceFile->treeWidgetItem());
    ui->treeWidget->setItemDelegate(new ZzzWorkspaceEditorRequestDelegate(this));

    connect(d->workspaceFile.data(), &WorkspaceFile::requestsChanged, this, [this] {
        this->updateRequests(d->workspaceFile->treeWidgetItem(), d->workspaceFile.dynamicCast<RequestContainerProvider>());
    });
    connect(d->workspaceFile.data(), &WorkspaceFile::dataChanged, this, [this] {
        if (!d->filePath.isEmpty()) {
            this->saveWorkspace(d->filePath);
        }

        ui->treeWidget->viewport()->update();
    });
    this->updateRequests(d->workspaceFile->treeWidgetItem(), d->workspaceFile.dynamicCast<RequestContainerProvider>());

    connect(&d->filesystemWatcher, &QFileSystemWatcher::fileChanged, this, [this](QString path) {
        if (d->filePath == path) {
            // Reload the file
            this->openWorkspace(d->filePath);
        }
    });

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
    d->filesystemWatcher.removePaths(d->filesystemWatcher.files());

    auto data = QJsonDocument(d->workspaceFile->toJson().toObject()).toJson();
    d->fileHash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);

    QFile file(filePath);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();

    d->filePath = filePath;
    d->filesystemWatcher.addPath(filePath);
}

void ZzzWorkspaceEditor::openWorkspace(QString filePath) {
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    auto data = file.readAll();
    file.close();

    // Only update the workspace if the file has actually changed
    auto newHash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    if (d->fileHash == newHash) {
        return;
    }

    QJsonParseError error;
    auto json = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        // TODO: Show an error
        return;
    }

    d->fileHash = newHash;

    d->workspaceFile->loadJson(json.object());

    d->filesystemWatcher.removePaths(d->filesystemWatcher.files());
    d->filePath = filePath;
    d->filesystemWatcher.addPath(filePath);
}

void ZzzWorkspaceEditor::on_newRequestButton_clicked() {
    d->workspaceFile->addRequest((new ZzzRequest(d->workspaceFile))->sharedFromThis());
}

void ZzzWorkspaceEditor::updateRequests(QTreeWidgetItem* rootItem, RequestContainerProviderPtr containerProvider) {
    auto items = tRange(containerProvider->requests()).map<QTreeWidgetItem*>([](ZzzRequestTreeItemPtr treeItem) {
                                                          return treeItem->treeWidgetItem();
                                                      })
                     .toList();

    // Remove extra children
    while (rootItem->childCount() > items.size()) {
        rootItem->takeChild(rootItem->childCount() - 1);
    }

    // Update existing children and add missing ones
    for (int i = 0; i < items.size(); ++i) {
        if (i < rootItem->childCount()) {
            // Update existing child
            if (rootItem->child(i) != items.at(i)) {
                rootItem->takeChild(i);
                rootItem->insertChild(i, items.at(i));
            }
        } else {
            // Add missing child
            rootItem->addChild(items.at(i));
        }
    }

    d->workspaceFile->treeWidgetItem()->setExpanded(true);
}

void ZzzWorkspaceEditor::on_treeWidget_itemSelectionChanged() {
    auto items = ui->treeWidget->selectedItems();
    if (items.length() == 0) {
    } else if (items.length() == 1) {
        auto request = items.constFirst()->data(0, Qt::UserRole).value<ZzzRequestTreeItemPtr>();

        auto editor = new ZzzRequestEditor(request);
        connect(editor, &ZzzRequestEditor::addReply, this, &ZzzWorkspaceEditor::addReply);
        ui->stackedWidget->addWidget(editor);
        ui->stackedWidget->setCurrentWidget(editor);
    } else {
    }
}

ZzzWorkspaceEditorRequestDelegate::ZzzWorkspaceEditorRequestDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {
}

tPaintCalculator ZzzWorkspaceEditorRequestDelegate::paintCalculator(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    auto size = QStyledItemDelegate::sizeHint(option, index);
    size.setWidth(option.widget->width());

    auto initialRect = option.rect;
    initialRect.setSize(size);

    tPaintCalculator paintCalculator;
    paintCalculator.setPainter(painter);
    paintCalculator.setDrawBounds(initialRect);
    paintCalculator.setLayoutDirection(option.direction);

    auto fontMetrics = option.fontMetrics;

    QRectF titleRect;
    titleRect.setHeight(fontMetrics.height());
    titleRect.setWidth(initialRect.width() - 12);
    titleRect.moveCenter(initialRect.center());
    titleRect.moveTop(initialRect.top() + 6);

    auto item = index.data(Qt::UserRole).value<ZzzRequestTreeItemPtr>();
    if (auto request = item.dynamicCast<ZzzRequest>()) {
        QRectF iconRect;
        iconRect.setSize({16, 16});
        iconRect.moveLeft(titleRect.left());
        iconRect.moveTop(titleRect.bottom() + 6);

        QRectF supplementaryTextRect;
        supplementaryTextRect.setHeight(fontMetrics.height());
        supplementaryTextRect.moveCenter(iconRect.center());
        supplementaryTextRect.moveLeft(iconRect.right() + 3);
        supplementaryTextRect.setRight(titleRect.right());

        paintCalculator.addRect(titleRect, [painter, index, option](QRectF drawBounds) {
            painter->setPen(option.palette.color(QPalette::WindowText));
            painter->drawText(drawBounds, index.data(Qt::DisplayRole).toString());
        });
        paintCalculator.addRect(iconRect, [painter, index, request](QRectF drawBounds) {
            auto icon = ZzzHelpers::iconForVerb(request->verb());
            painter->drawPixmap(drawBounds.toRect(), icon.pixmap(drawBounds.size().toSize()));
        });
        paintCalculator.addRect(supplementaryTextRect, [painter, index, option, request, fontMetrics](QRectF drawBounds) {
            auto text = fontMetrics.elidedText(request->calculateUrl().path(), Qt::ElideRight, drawBounds.width());
            if (text.isEmpty()) text = QStringLiteral("/");

            painter->save();
            painter->setPen(option.palette.color(QPalette::Disabled, QPalette::WindowText));
            painter->drawText(drawBounds, text);
            painter->restore();
        });
    } else if (auto workspace = item.dynamicCast<WorkspaceFile>()) {
        paintCalculator.addRect(titleRect, [painter, index, option](QRectF drawBounds) {
            painter->setPen(option.palette.color(QPalette::WindowText));
            painter->drawText(drawBounds, index.data(Qt::DisplayRole).toString());
        });
    }

    return paintCalculator;
}

void ZzzWorkspaceEditorRequestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    auto paintCalculator = this->paintCalculator(painter, option, index);
    paintCalculator.setPainter(painter);
    paintCalculator.performPaint();
}

QSize ZzzWorkspaceEditorRequestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    auto paintCalculator = this->paintCalculator(nullptr, option, index);
    auto size = paintCalculator.sizeWithMargins().toSize();
    return size;
}

void ZzzWorkspaceEditor::on_treeWidget_customContextMenuRequested(const QPoint& pos) {
    auto item = ui->treeWidget->itemAt(pos);
    if (item) {
        auto treeItem = item->data(0, Qt::UserRole).value<ZzzRequestTreeItemPtr>();

        auto menu = new QMenu(this);
        menu->addSection(tr("For %1").arg(QLocale().quoteString(item->text(0))));

        if (auto container = treeItem.dynamicCast<RequestContainerProvider>()) {
            menu->addAction(QIcon::fromTheme("list-add"), tr("New Request"), [treeItem, this, container] {
                container->addRequest((new ZzzRequest(d->workspaceFile))->sharedFromThis());
            });
        }

        if (item != d->workspaceFile->treeWidgetItem()) {
            menu->addAction(QIcon::fromTheme("edit-delete"), tr("Remove"), [treeItem, this] {
                d->workspaceFile->removeRequestRecursive(treeItem);
            });
        }

        connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
        menu->popup(ui->treeWidget->mapToGlobal(pos));
    }
}
