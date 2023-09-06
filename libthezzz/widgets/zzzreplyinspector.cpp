#include "zzzreplyinspector.h"
#include "ui_zzzreplyinspector.h"

#include "replyinspector/replybodywidget.h"
#include "replyinspector/replyheaderswidget.h"
#include "zzzreply.h"
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

    this->addReplyInspectorWidget(new ReplyHeadersWidget(reply, this));
    this->addReplyInspectorWidget(new ReplyBodyWidget(reply, this));
    ui->stackedWidget->setCurrentIndex(0);

    connect(reply.data(), &ZzzReply::updated, this, &ZzzReplyInspector::updateReply);
    this->updateReply();
}

ZzzReplyInspector::~ZzzReplyInspector() {
    delete d;
    delete ui;
}

void ZzzReplyInspector::updateReply() {
    if (d->reply->finished()) {
        auto statusCode = d->reply->statusCode();

        ui->statusCodeLabel->setVisible(true);
        ui->timingsLabel->setVisible(true);
        ui->statusCodeLabel->setText(QString::number(statusCode));
        ui->statusTextLabel->setText(d->reply->reasonPhrase());
        ui->timingsLabel->setText(tr("in %n msec", nullptr, d->reply->requestTime()));

        QPalette pal = ui->statusCodeWidget->palette();
        if (statusCode <= 199) {
            pal.setColor(QPalette::Window, QColor(0, 100, 255));
            pal.setColor(QPalette::WindowText, Qt::white);
        } else if (statusCode >= 200 && statusCode <= 299) {
            pal.setColor(QPalette::Window, QColor(0, 100, 0));
            pal.setColor(QPalette::WindowText, Qt::white);
        } else if (statusCode >= 300 && statusCode <= 399) {
            pal.setColor(QPalette::Window, QColor(0, 100, 255));
            pal.setColor(QPalette::WindowText, Qt::white);
        } else {
            pal.setColor(QPalette::Window, QColor(100, 0, 0));
            pal.setColor(QPalette::WindowText, Qt::white);
        }
        ui->statusCodeWidget->setPalette(pal);
        ui->statusCodeWidget->setAutoFillBackground(true);
    } else {
        ui->statusCodeLabel->setVisible(false);
        ui->timingsLabel->setVisible(false);
        ui->statusTextLabel->setText(tr("Processing..."));
        ui->statusCodeWidget->setAutoFillBackground(false);
    }
}

void ZzzReplyInspector::addReplyInspectorWidget(ReplyInspectorWidget* widget) {
    ui->stackedWidget->addWidget(widget);

    auto button = new QToolButton();
    button->setText(widget->text());
    button->setCheckable(true);
    button->setAutoExclusive(true);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(button, &QToolButton::clicked, this, [this, widget] {
        ui->stackedWidget->setCurrentWidget(widget);
    });
    ui->buttonsLayout->addWidget(button);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this, button, widget] {
        button->setChecked(ui->stackedWidget->currentWidget() == widget);
    });
}
