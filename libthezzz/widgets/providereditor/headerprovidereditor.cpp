#include "headerprovidereditor.h"
#include "ui_headerprovidereditor.h"

#include "providers/headersprovider.h"
#include "workspacefile.h"
#include <ranges/trange.h>

struct HeaderProviderEditorPrivate {
        HeadersProvider* headersProvider;
};

HeaderProviderEditor::HeaderProviderEditor(HeadersProvider* headersProvider, QWidget* parent) :
    ProviderEditor(headersProvider->workspace()->sharedFromThis().staticCast<WorkspaceFile>(), parent),
    ui(new Ui::HeaderProviderEditor) {
    ui->setupUi(this);
    d = new HeaderProviderEditorPrivate();
    d->headersProvider = headersProvider;
}

HeaderProviderEditor::~HeaderProviderEditor() {
    delete ui;
    delete d;
}

QString HeaderProviderEditor::text() {
    return tr("Hdrs", "Short for Headers");
}

int HeaderProviderEditor::order() {
    return 10;
}

void HeaderProviderEditor::updateData() {
    ui->headersList->clear();

    ZzzHeaders implicitHeaders = d->headersProvider->implicitHeadersWithAncestors();
    ZzzHeaders parentHeaders = d->headersProvider->ancestorHeaders();
    ZzzHeaders thisHeaders = d->headersProvider->headers();

    for (const auto& header : parentHeaders) {
        implicitHeaders.removeIf([header](ZzzHeader existingHeader) {
            return existingHeader.first.toLower() == header.first.toLower();
        });
    }

    for (const auto& header : implicitHeaders) {
        auto item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, false);
        item->setText(0, header.first);
        item->setText(1, header.second);
        item->setFlags(Qt::ItemIsSelectable);
        item->setForeground(0, this->palette().color(QPalette::Disabled, QPalette::WindowText));
        item->setForeground(1, this->palette().color(QPalette::Disabled, QPalette::WindowText));
        if (tRange(thisHeaders).some([header](const ZzzHeader& existingHeader) {
                return existingHeader.first.toLower() == header.first.toLower();
            })) {
            auto font = item->font(0);
            font.setStrikeOut(true);
            item->setFont(0, font);
            item->setFont(1, font);
        }
        ui->headersList->addTopLevelItem(item);
    }

    for (const auto& header : parentHeaders) {
        auto item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, false);
        item->setText(0, header.first);
        item->setText(1, header.second);
        item->setFlags(Qt::ItemIsSelectable);
        item->setForeground(0, this->palette().color(QPalette::Disabled, QPalette::WindowText));
        item->setForeground(1, this->palette().color(QPalette::Disabled, QPalette::WindowText));
        if (tRange(thisHeaders).some([header](const ZzzHeader& existingHeader) {
                return existingHeader.first.toLower() == header.first.toLower();
            })) {
            auto font = item->font(0);
            font.setStrikeOut(true);
            item->setFont(0, font);
            item->setFont(1, font);
        }
        ui->headersList->addTopLevelItem(item);
    }

    for (const auto& header : thisHeaders) {
        auto item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, true);
        item->setText(0, header.first);
        item->setText(1, header.second);
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->headersList->addTopLevelItem(item);
    }

    auto newItem = new QTreeWidgetItem();
    newItem->setData(0, Qt::UserRole, true);
    newItem->setText(0, tr("New Header..."));
    newItem->setForeground(0, this->palette().color(QPalette::Disabled, QPalette::Text));
    newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->headersList->addTopLevelItem(newItem);
}

void HeaderProviderEditor::on_headersList_itemChanged(QTreeWidgetItem* item, int column) {
    ZzzHeaders headers;
    for (auto i = 0; i < ui->headersList->topLevelItemCount(); i++) {
        auto item = ui->headersList->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toBool() && !item->text(0).isEmpty() && item->text(0) != tr("New Header...")) {
            headers.append({item->text(0).toUtf8(), item->text(1).toUtf8()});
        }
    }

    d->headersProvider->setHeaders(headers);
}
