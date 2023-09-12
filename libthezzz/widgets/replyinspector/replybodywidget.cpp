#include "replybodywidget.h"
#include "ui_replybodywidget.h"

#include "zzzreply.h"
#include <QFontDatabase>
#include <texteditor.h>

struct ReplyBodyWidgetPrivate {
};

ReplyBodyWidget::ReplyBodyWidget(ZzzReplyPtr reply, QWidget* parent) :
    ReplyInspectorWidget(std::move(reply), parent),
    ui(new Ui::ReplyBodyWidget) {
    ui->setupUi(this);
    d = new ReplyBodyWidgetPrivate();

    ui->editor->editor()->setReadOnly(true);
}

ReplyBodyWidget::~ReplyBodyWidget() {
    delete ui;
    delete d;
}

QString ReplyBodyWidget::text() {
    return tr("Raw", "Short for Raw Body");
}

void ReplyBodyWidget::updateReply() {
    ui->editor->editor()->setText(reply()->body());
}
