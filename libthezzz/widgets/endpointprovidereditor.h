#ifndef ENDPOINTPROVIDEREDITOR_H
#define ENDPOINTPROVIDEREDITOR_H

#include "forwarddeclares.h"
#include <QWidget>

namespace Ui {
    class EndpointProviderEditor;
}

struct EndpointProviderEditorPrivate;
class EndpointProviderEditor : public QWidget {
        Q_OBJECT

    public:
        explicit EndpointProviderEditor(QWidget* parent = nullptr);
        ~EndpointProviderEditor();

        void setEndpointProvider(EndpointProviderPtr endpointProvider);

    private slots:
        void on_endpointEdit_textChanged(const QString& arg1);

        void on_method_currentTextChanged(const QString& arg1);

    private:
        Ui::EndpointProviderEditor* ui;
        EndpointProviderEditorPrivate* d;
};

#endif // ENDPOINTPROVIDEREDITOR_H
