#include "jsonbodywidget.h"
#include "ui_jsonbodywidget.h"

#include "zzzreply.h"
#include <QJsonDocument>
#include <texteditor.h>

JsonBodyWidget::JsonBodyWidget(ZzzReplyPtr reply, QWidget* parent) :
    ReplyInspectorWidget(std::move(reply), parent),
    ui(new Ui::JsonBodyWidget) {
    ui->setupUi(this);

    ui->editor->editor()->setReadOnly(true);
}

JsonBodyWidget::~JsonBodyWidget() {
    delete ui;
}

QString JsonBodyWidget::text() {
    return tr("JSON");
}

void JsonBodyWidget::updateReply() {
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(reply()->body(), &error);

    if (error.error != QJsonParseError::NoError) {
        ui->editor->editor()->setText(tr("Unable to parse JSON\nError: %1\nat %2").arg(error.errorString()).arg(error.offset));
    } else {
        ui->editor->editor()->setText(doc.toJson());
    }
}
