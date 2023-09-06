#include "headerprovidereditor.h"
#include "ui_headerprovidereditor.h"

#include "providers/headersprovider.h"
#include "workspacefile.h"

struct HeaderProviderEditorPrivate {
        HeadersProvider* headersProvider;
};

HeaderProviderEditor::HeaderProviderEditor(HeadersProvider* headersProvider, QWidget* parent) :
    ProviderEditor(headersProvider->workspace()->sharedFromThis(), parent),
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

    auto providesBase = dynamic_cast<ZzzProvidesBase*>(d->headersProvider);
    for (auto header : providesBase->allImplicitHeaders()) {
        auto item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, false);
        item->setText(0, header.first);
        item->setText(1, header.second);
        item->setFlags(Qt::ItemIsSelectable);
        item->setForeground(0, this->palette().color(QPalette::Disabled, QPalette::WindowText));
        item->setForeground(1, this->palette().color(QPalette::Disabled, QPalette::WindowText));
        ui->headersList->addTopLevelItem(item);
    }

    for (auto header : d->headersProvider->headers()) {
        auto item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, true);
        item->setText(0, header.first);
        item->setText(1, header.second);
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->headersList->addTopLevelItem(item);
    }

    auto newItem = new QTreeWidgetItem();
    newItem->setData(0, Qt::UserRole, true);
    newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->headersList->addTopLevelItem(newItem);
}

void HeaderProviderEditor::on_headersList_itemChanged(QTreeWidgetItem* item, int column) {
    ZzzHeaders headers;
    for (auto i = 0; i < ui->headersList->topLevelItemCount(); i++) {
        auto item = ui->headersList->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toBool() && !item->text(0).isEmpty()) {
            headers.append({item->text(0).toUtf8(), item->text(1).toUtf8()});
        }
    }

    d->headersProvider->setHeaders(headers);
}
