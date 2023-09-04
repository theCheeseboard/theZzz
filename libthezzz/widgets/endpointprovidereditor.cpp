#include "endpointprovidereditor.h"
#include "ui_endpointprovidereditor.h"

#include "providers/endpointprovider.h"

struct EndpointProviderEditorPrivate {
        EndpointProviderPtr endpointProvider;
};

EndpointProviderEditor::EndpointProviderEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::EndpointProviderEditor) {
    ui->setupUi(this);

    d = new EndpointProviderEditorPrivate();
}

EndpointProviderEditor::~EndpointProviderEditor() {
    delete d;
    delete ui;
}

void EndpointProviderEditor::setEndpointProvider(EndpointProviderPtr endpointProvider) {
    d->endpointProvider = endpointProvider;
    ui->endpointEdit->setText(endpointProvider->endpoint());
    ui->method->setCurrentText(endpointProvider->verb());
}

void EndpointProviderEditor::on_endpointEdit_textChanged(const QString& arg1) {
    d->endpointProvider->setEndpoint(arg1);
}

void EndpointProviderEditor::on_method_currentTextChanged(const QString& arg1) {
    d->endpointProvider->setVerb(arg1);
}
