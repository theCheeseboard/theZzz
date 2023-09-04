#include "zzzrequesteditor.h"
#include "ui_zzzrequesteditor.h"

#include "providers/endpointprovider.h"
#include "zzzrequest.h"

struct ZzzRequestEditorPrivate {
        ZzzRequestPtr request;
};

ZzzRequestEditor::ZzzRequestEditor(ZzzRequestPtr request, QWidget* parent) :
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
}

ZzzRequestEditor::~ZzzRequestEditor() {
    delete d;
    delete ui;
}
