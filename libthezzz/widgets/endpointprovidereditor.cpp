#include "endpointprovidereditor.h"
#include "ui_endpointprovidereditor.h"

#include "providers/endpointprovider.h"
#include "workspacefile.h"

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

    connect(endpointProvider->workspace(), &WorkspaceFile::dataChanged, this, &EndpointProviderEditor::updateData);
    this->updateData();
}

void EndpointProviderEditor::on_endpointEdit_textChanged(const QString& arg1) {
    d->endpointProvider->setEndpoint(arg1);
}

void EndpointProviderEditor::on_method_currentTextChanged(const QString& arg1) {
    d->endpointProvider->setVerb(arg1);
}

void EndpointProviderEditor::on_sendButton_clicked() {
    emit execute();
}

void EndpointProviderEditor::updateData() {
    if (!ui->endpointEdit->hasFocus()) {
        ui->endpointEdit->setText(d->endpointProvider->endpoint());
    }

    if (!ui->method->hasFocus()) {
        auto index = ui->method->findText(d->endpointProvider->verb());
        if (index == -1) {
            ui->method->setCurrentText(d->endpointProvider->verb());
        } else {
            ui->method->setCurrentIndex(index);
        }
    }
}
