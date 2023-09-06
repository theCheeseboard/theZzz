#include "zzzrequesteditor.h"
#include "ui_zzzrequesteditor.h"

#include "providereditor/providereditor.h"
#include "providers/endpointprovider.h"
#include "zzzrequest.h"
#include <QToolButton>

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

    if (auto base = request.dynamicCast<ZzzProvidesBase>()) {
        auto editors = base->editors();
        std::sort(editors.begin(), editors.end(), [](ProviderEditor* first, ProviderEditor* second) {
            return first->order() < second->order();
        });

        for (auto editor : editors) {
            ui->stackedWidget->addWidget(editor);

            auto button = new QToolButton();
            button->setText(editor->text());
            button->setCheckable(true);
            button->setAutoExclusive(true);
            button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            connect(button, &QToolButton::clicked, this, [this, editor] {
                ui->stackedWidget->setCurrentWidget(editor);
            });
            ui->buttonsLayout->addWidget(button);

            connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this, button, editor] {
                button->setChecked(ui->stackedWidget->currentWidget() == editor);
            });
        }
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
