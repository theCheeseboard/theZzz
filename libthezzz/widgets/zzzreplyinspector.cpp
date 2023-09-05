#include "zzzreplyinspector.h"
#include "ui_zzzreplyinspector.h"

#include "replyinspector/replybodywidget.h"
#include <QToolButton>

struct ZzzReplyInspectorPrivate {
        ZzzReplyPtr reply;
};

ZzzReplyInspector::ZzzReplyInspector(ZzzReplyPtr reply, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ZzzReplyInspector) {
    ui->setupUi(this);
    d = new ZzzReplyInspectorPrivate();
    d->reply = reply;

    auto bodyInspector = new ReplyBodyWidget(reply, this);
    ui->stackedWidget->addWidget(bodyInspector);

    auto bodyButton = new QToolButton();
    bodyButton->setText(QStringLiteral("bdy"));
    bodyButton->setCheckable(true);
    bodyButton->setAutoExclusive(true);
    bodyButton->setChecked(true);
    connect(bodyButton, &QToolButton::triggered, this, [this, bodyInspector] {
        ui->stackedWidget->setCurrentWidget(bodyInspector);
    });
    ui->buttonsLayout->addWidget(bodyButton);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this, bodyButton, bodyInspector] {
        auto widget = ui->stackedWidget->currentWidget();
        if (widget == bodyInspector) {
            bodyButton->setChecked(true);
        }
    });
}

ZzzReplyInspector::~ZzzReplyInspector() {
    delete d;
    delete ui;
}
