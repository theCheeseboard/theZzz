#include "bodyprovidereditor.h"
#include "ui_bodyprovidereditor.h"

#include "providers/bodyprovider.h"
#include "workspacefile.h"

struct BodyProviderEditorPrivate {
        BodyProvider* provider;
};

BodyProviderEditor::BodyProviderEditor(BodyProvider* bodyProvider, QWidget* parent) :
    ProviderEditor(bodyProvider->workspace()->sharedFromThis(), parent),
    ui(new Ui::BodyProviderEditor) {
    ui->setupUi(this);

    d = new BodyProviderEditorPrivate();
    d->provider = bodyProvider;
}

BodyProviderEditor::~BodyProviderEditor() {
    delete ui;
    delete d;
}

QString BodyProviderEditor::text() {
    return tr("Body", "Short for Body");
}

int BodyProviderEditor::order() {
    return 200;
}

void BodyProviderEditor::on_plainTextEdit_textChanged() {
    d->provider->setBody(ui->plainTextEdit->toPlainText().toUtf8());
}

void BodyProviderEditor::updateData() {
    if (ui->plainTextEdit->hasFocus()) return;

    QSignalBlocker blocker(ui->plainTextEdit);
    ui->plainTextEdit->setPlainText(d->provider->body());
}
