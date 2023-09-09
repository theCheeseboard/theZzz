#ifndef ENVIRONMENTPROVIDEREDITOR_H
#define ENVIRONMENTPROVIDEREDITOR_H

#include "providereditor.h"

namespace Ui {
    class EnvironmentProviderEditor;
}

class QTableWidgetItem;
class EnvironmentProvider;
struct EnvironmentProviderEditorPrivate;
class EnvironmentProviderEditor : public ProviderEditor {
        Q_OBJECT

    public:
        explicit EnvironmentProviderEditor(EnvironmentProvider* environmentProvider, QWidget* parent = nullptr);
        ~EnvironmentProviderEditor();

    private:
        Ui::EnvironmentProviderEditor* ui;
        EnvironmentProviderEditorPrivate* d;

        // ProviderEditor interface
    public:
        QString text();
        int order();

    protected:
        void updateData();
    private slots:
        void on_tableWidget_itemChanged(QTableWidgetItem* item);
};

#endif // ENVIRONMENTPROVIDEREDITOR_H
