#include "replybodywidget.h"
#include "ui_replybodywidget.h"

#include "zzzreply.h"
#include <QFontDatabase>

struct ReplyBodyWidgetPrivate {
        ZzzReplyPtr reply;
};

ReplyBodyWidget::ReplyBodyWidget(ZzzReplyPtr reply, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ReplyBodyWidget) {
    ui->setupUi(this);
    d = new ReplyBodyWidgetPrivate();
    d->reply = reply;

    ui->bodyText->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    connect(reply.data(), &ZzzReply::updated, this, &ReplyBodyWidget::updateReply);
    this->updateReply();
}

ReplyBodyWidget::~ReplyBodyWidget() {
    delete ui;
    delete d;
}

void ReplyBodyWidget::updateReply() {
    ui->bodyText->setPlainText(d->reply->body());
}
