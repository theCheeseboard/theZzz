#include "zzzreplyviewer.h"
#include "ui_zzzreplyviewer.h"

#include "zzzhelpers.h"
#include "zzzreply.h"
#include "zzzreplyinspector.h"
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
        ui->replyList->insertItem(0, QStringLiteral("%1").arg(reply->url().toString()));
        ui->replyList->setItemIcon(0, ZzzHelpers::iconForVerb(reply->verb()));

        auto inspector = new ZzzReplyInspector(reply);
        ui->stackedWidget->insertWidget(0, inspector);

        ui->replyList->setCurrentIndex(0);
        ui->stackedWidget->setCurrentWidget(inspector);
    });
}

ZzzReplyViewer::~ZzzReplyViewer() {
    delete d;
    delete ui;
}

ZzzReplyManagerPtr ZzzReplyViewer::replyManager() {
    return d->replyManager;
}

void ZzzReplyViewer::on_replyList_currentIndexChanged(int index) {
    ui->stackedWidget->setCurrentIndex(index);
}
