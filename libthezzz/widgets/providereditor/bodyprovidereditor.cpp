#include "bodyprovidereditor.h"
#include "ui_bodyprovidereditor.h"

#include "providers/bodyprovider.h"
#include "workspacefile.h"
#include <ttexteditor/texteditor.h>

struct BodyProviderEditorPrivate {
        BodyProvider* provider;
};

BodyProviderEditor::BodyProviderEditor(BodyProvider* bodyProvider, QWidget* parent) :
    ProviderEditor(bodyProvider->workspace()->sharedFromThis().staticCast<WorkspaceFile>(), parent),
    ui(new Ui::BodyProviderEditor) {
    ui->setupUi(this);

    d = new BodyProviderEditorPrivate();
    d->provider = bodyProvider;

    connect(ui->editor->editor(), &TextEditor::textChanged, this, [this] {
        d->provider->setBody(ui->editor->editor()->text().toUtf8());
    });
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

void BodyProviderEditor::updateData() {
    if (ui->editor->editor()->hasFocus()) return;

    QSignalBlocker blocker(ui->editor->editor());
    ui->editor->editor()->setText(d->provider->body());
}
