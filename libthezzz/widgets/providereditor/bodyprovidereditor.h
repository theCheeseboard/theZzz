#ifndef BODYPROVIDEREDITOR_H
#define BODYPROVIDEREDITOR_H

#include "providereditor.h"

namespace Ui {
    class BodyProviderEditor;
}

class BodyProvider;
struct BodyProviderEditorPrivate;
class BodyProviderEditor : public ProviderEditor {
        Q_OBJECT

    public:
        explicit BodyProviderEditor(BodyProvider* bodyProvider, QWidget* parent = nullptr);
        ~BodyProviderEditor();

        QString text();
        int order();

    protected:
        void updateData();

    private slots:
        void on_actionCustom_triggered();

        void on_actionForm_Data_triggered();

    private:
        Ui::BodyProviderEditor* ui;
        BodyProviderEditorPrivate* d;
};

#endif // BODYPROVIDEREDITOR_H
