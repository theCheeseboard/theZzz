#ifndef SETTINGSPROVIDEREDITOR_H
#define SETTINGSPROVIDEREDITOR_H

#include "providereditor.h"
#include <QWidget>

namespace Ui {
    class SettingsProviderEditor;
}

class SettingsProvider;
struct SettingsProviderEditorPrivate;
class SettingsProviderEditor : public ProviderEditor {
        Q_OBJECT

    public:
        explicit SettingsProviderEditor(SettingsProvider* settingsProvider, QWidget* parent = nullptr);
        ~SettingsProviderEditor();

    private:
        Ui::SettingsProviderEditor* ui;

        SettingsProviderEditorPrivate* d;

        // ProviderEditor interface
    public:
        QString text();
        int order();

    protected:
        void updateData();
    private slots:
        void on_skipTlsCheckbox_toggled(bool checked);
};

#endif // SETTINGSPROVIDEREDITOR_H
