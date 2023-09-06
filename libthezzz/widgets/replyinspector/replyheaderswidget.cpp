#include "replyheaderswidget.h"
#include "ui_replyheaderswidget.h"

#include "zzzreply.h"
#include <QClipboard>
#include <QMenu>

ReplyHeadersWidget::ReplyHeadersWidget(ZzzReplyPtr reply, QWidget* parent) :
    ReplyInspectorWidget(std::move(reply), parent), ui(new Ui::ReplyHeadersWidget) {
    ui->setupUi(this);
}

ReplyHeadersWidget::~ReplyHeadersWidget() {
    delete ui;
}

QString ReplyHeadersWidget::text() {
    return tr("Hdr", "Short for Headers");
}

void ReplyHeadersWidget::updateReply() {
    ui->headersList->clear();
    for (const auto& header : reply()->headers()) {
        auto item = new QTreeWidgetItem();

        item->setText(0, header.first);
        item->setText(1, header.second);

        ui->headersList->addTopLevelItem(item);
    }
}

void ReplyHeadersWidget::on_headersList_customContextMenuRequested(const QPoint& pos) {
    auto header = ui->headersList->itemAt(pos);
    if (header) {
        auto menu = new QMenu(this);
        menu->addSection(tr("For header %1").arg(QLocale().quoteString(header->text(0))));
        menu->addAction(QIcon::fromTheme("edit-copy"), tr("Copy Header"), [header] {
            qApp->clipboard()->setText(header->text(0));
        });
        menu->addAction(QIcon::fromTheme("edit-copy"), tr("Copy Value"), [header] {
            qApp->clipboard()->setText(header->text(1));
        });
        connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
        menu->popup(ui->headersList->mapToGlobal(pos));
    }
}
