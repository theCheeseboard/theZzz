#ifndef ENDPOINTPROVIDEREDITOR_H
#define ENDPOINTPROVIDEREDITOR_H

#include "forwarddeclares.h"
#include <QWidget>
#include "libthezzz_global.h"

namespace Ui {
    class EndpointProviderEditor;
}

struct EndpointProviderEditorPrivate;
class LIBTHEZZZ_EXPORT EndpointProviderEditor : public QWidget {
        Q_OBJECT

    public:
        explicit EndpointProviderEditor(QWidget* parent = nullptr);
        ~EndpointProviderEditor();

        void setEndpointProvider(EndpointProviderPtr endpointProvider);

    private slots:
        void on_endpointEdit_textChanged(const QString& arg1);

        void on_method_currentTextChanged(const QString& arg1);

        void on_sendButton_clicked();

    signals:
        void execute();

    private:
        Ui::EndpointProviderEditor* ui;
        EndpointProviderEditorPrivate* d;
};

#endif // ENDPOINTPROVIDEREDITOR_H
