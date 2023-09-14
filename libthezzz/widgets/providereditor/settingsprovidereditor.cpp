#include "settingsprovidereditor.h"
#include "ui_settingsprovidereditor.h"

#include "providers/settingsprovider.h"
#include "workspacefile.h"

struct SettingsProviderEditorPrivate {
        SettingsProvider* settingsProvider;
};

SettingsProviderEditor::SettingsProviderEditor(SettingsProvider* settingsProvider, QWidget* parent) :
    ProviderEditor(settingsProvider->workspace()->sharedFromThis().staticCast<WorkspaceFile>()),
    ui(new Ui::SettingsProviderEditor) {
    ui->setupUi(this);
    d = new SettingsProviderEditorPrivate();
    d->settingsProvider = settingsProvider;
}

SettingsProviderEditor::~SettingsProviderEditor() {
    delete d;
    delete ui;
}

QString SettingsProviderEditor::text() {
    return tr("Stgs", "Short for Settings");
}

int SettingsProviderEditor::order() {
    return 400;
}

void SettingsProviderEditor::updateData() {
    ui->skipTlsCheckbox->setChecked(d->settingsProvider->skipTlsVerification());
}

void SettingsProviderEditor::on_skipTlsCheckbox_toggled(bool checked) {
    d->settingsProvider->setSkipTlsVerification(checked);
}
