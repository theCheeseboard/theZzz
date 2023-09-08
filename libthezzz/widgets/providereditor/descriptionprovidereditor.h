#ifndef DESCRIPTIONPROVIDEREDITOR_H
#define DESCRIPTIONPROVIDEREDITOR_H

#include "providereditor.h"

namespace Ui {
    class DescriptionProviderEditor;
}

class DescriptionProvider;
struct DescriptionProviderEditorPrivate;
class DescriptionProviderEditor : public ProviderEditor {
        Q_OBJECT

    public:
        explicit DescriptionProviderEditor(DescriptionProvider* descriptionProvider, QWidget* parent = nullptr);
        ~DescriptionProviderEditor();

    private:
        Ui::DescriptionProviderEditor* ui;
        DescriptionProviderEditorPrivate* d;

        // ProviderEditor interface
    public:
        QString text();
        int order();

    protected:
        void updateData();
    private slots:
        void on_titleEdit_textChanged(const QString& arg1);
        void on_descriptionEdit_textChanged();
};

#endif // DESCRIPTIONPROVIDEREDITOR_H
