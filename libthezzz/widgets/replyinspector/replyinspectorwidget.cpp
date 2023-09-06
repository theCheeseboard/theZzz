#include "replyinspectorwidget.h"

#include "zzzreply.h"
#include <QTimer>

struct ReplyInspectorWidgetPrivate {
        ZzzReplyPtr reply;
};

ReplyInspectorWidget::ReplyInspectorWidget(ZzzReplyPtr reply, QWidget* parent) :
    QWidget(parent) {
    d = new ReplyInspectorWidgetPrivate();
    d->reply = reply;

    connect(reply.data(), &ZzzReply::updated, this, &ReplyInspectorWidget::updateReply);
    QTimer::singleShot(0, this, &ReplyInspectorWidget::updateReply);
}

ReplyInspectorWidget::~ReplyInspectorWidget() {
    delete d;
}

ZzzReplyPtr ReplyInspectorWidget::reply() {
    return d->reply;
}
