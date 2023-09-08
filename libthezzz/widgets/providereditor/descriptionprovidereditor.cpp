#include "descriptionprovidereditor.h"
#include "ui_descriptionprovidereditor.h"

#include "providers/descriptionprovider.h"
#include "workspacefile.h"

struct DescriptionProviderEditorPrivate {
        DescriptionProvider* provider;
};

DescriptionProviderEditor::DescriptionProviderEditor(DescriptionProvider* descriptionProvider, QWidget* parent) :
    ProviderEditor(descriptionProvider->workspace()->sharedFromThis().staticCast<WorkspaceFile>(), parent),
    ui(new Ui::DescriptionProviderEditor) {
    ui->setupUi(this);
    d = new DescriptionProviderEditorPrivate();
    d->provider = descriptionProvider;
}

DescriptionProviderEditor::~DescriptionProviderEditor() {
    delete ui;
    delete d;
}

QString DescriptionProviderEditor::text() {
    return tr("Desc", "Short for Description");
}

int DescriptionProviderEditor::order() {
    return 0;
}

void DescriptionProviderEditor::updateData() {
    if (ui->titleEdit->hasFocus()) return;
    if (ui->descriptionEdit->hasFocus()) return;

    QSignalBlocker titleBlocker(ui->titleEdit);
    QSignalBlocker descriptionBlocker(ui->descriptionEdit);
    ui->titleEdit->setText(d->provider->title());
    ui->descriptionEdit->setPlainText(d->provider->description());
}

void DescriptionProviderEditor::on_titleEdit_textChanged(const QString& arg1) {
    d->provider->setTitle(arg1);
}

void DescriptionProviderEditor::on_descriptionEdit_textChanged() {
    d->provider->setDescription(ui->descriptionEdit->toPlainText());
}
