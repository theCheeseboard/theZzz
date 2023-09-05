#include "zzzrequesteditor.h"
#include "ui_zzzrequesteditor.h"

#include "providers/endpointprovider.h"

#include "zzzrequest.h"

struct ZzzRequestEditorPrivate {
        QSharedPointer<QObject> request;
};

ZzzRequestEditor::ZzzRequestEditor(QSharedPointer<QObject> request, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ZzzRequestEditor) {
    ui->setupUi(this);
    d = new ZzzRequestEditorPrivate();
    d->request = request;

    if (auto endpointProvider = request.dynamicCast<EndpointProvider>()) {
        ui->endpointProvider->setEndpointProvider(endpointProvider);
    } else {
        ui->endpointProvider->setVisible(false);
    }

    connect(ui->endpointProvider, &EndpointProviderEditor::execute, this, &ZzzRequestEditor::executeRequest);
}

ZzzRequestEditor::~ZzzRequestEditor() {
    delete d;
    delete ui;
}

void ZzzRequestEditor::executeRequest() {
    auto request = d->request.dynamicCast<ZzzRequest>();
    if (!request) return;

    auto reply = request->execute();
    emit addReply(reply);
}
