#include "zzzreplyviewer.h"
#include "ui_zzzreplyviewer.h"

#include "zzzreply.h"
#include "zzzreplymanager.h"

struct ZzzReplyViewerPrivate {
        ZzzReplyManagerPtr replyManager;
};

ZzzReplyViewer::ZzzReplyViewer(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ZzzReplyViewer) {
    ui->setupUi(this);
    d = new ZzzReplyViewerPrivate();
    d->replyManager = (new ZzzReplyManager())->sharedFromThis();

    connect(d->replyManager.data(), &ZzzReplyManager::newReply, this, [this](ZzzReplyPtr reply) {
        ui->replyList->addItem(QStringLiteral("%1 %2").arg(reply->verb(), reply->url().toString()));
    });
}

ZzzReplyViewer::~ZzzReplyViewer() {
    delete d;
    delete ui;
}

ZzzReplyManagerPtr ZzzReplyViewer::replyManager() {
    return d->replyManager;
}
