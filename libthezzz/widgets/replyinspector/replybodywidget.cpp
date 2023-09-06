#include "replybodywidget.h"
#include "ui_replybodywidget.h"

#include "zzzreply.h"
#include <QFontDatabase>

struct ReplyBodyWidgetPrivate {
};

ReplyBodyWidget::ReplyBodyWidget(ZzzReplyPtr reply, QWidget* parent) :
    ReplyInspectorWidget(std::move(reply), parent),
    ui(new Ui::ReplyBodyWidget) {
    ui->setupUi(this);
    d = new ReplyBodyWidgetPrivate();

    ui->bodyText->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

ReplyBodyWidget::~ReplyBodyWidget() {
    delete ui;
    delete d;
}

QString ReplyBodyWidget::text() {
    return tr("Raw", "Short for Raw Body");
}

void ReplyBodyWidget::updateReply() {
    ui->bodyText->setPlainText(reply()->body());
}
