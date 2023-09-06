#include "zzzworkspaceeditor.h"
#include "ui_zzzworkspaceeditor.h"

#include "ranges/trange.h"
#include "zzzhelpers.h"
#include "zzzrequest.h"
#include "zzzrequesteditor.h"
#include <QFile>
#include <QJsonDocument>
#include <QPainter>
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
    ui->treeWidget->setItemDelegate(new ZzzWorkspaceEditorRequestDelegate(this));

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
    auto data = QJsonDocument(d->workspaceFile->toJson().toObject()).toJson();

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

    if (auto request = index.data(Qt::UserRole).value<ZzzRequestPtr>()) {
        QRectF titleRect;
        titleRect.setHeight(fontMetrics.height());
        titleRect.setWidth(initialRect.width() - 6);
        titleRect.moveCenter(initialRect.center());

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
            auto text = fontMetrics.elidedText(request->endpoint(), Qt::ElideRight, drawBounds.width());

            painter->save();
            painter->setPen(option.palette.color(QPalette::Disabled, QPalette::WindowText));
            painter->drawText(drawBounds, text);
            painter->restore();
        });
    } else if (auto workspace = index.data(Qt::UserRole).value<WorkspaceFilePtr>()) {
        QRectF titleRect;
        titleRect.setHeight(fontMetrics.height());
        titleRect.setWidth(initialRect.width() - 6);
        titleRect.moveCenter(initialRect.center());

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
