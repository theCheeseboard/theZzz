#ifndef REPLYHEADERSWIDGET_H
#define REPLYHEADERSWIDGET_H

#include "replyinspectorwidget.h"

namespace Ui {
    class ReplyHeadersWidget;
}

class ReplyHeadersWidget : public ReplyInspectorWidget {
        Q_OBJECT

    public:
        explicit ReplyHeadersWidget(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~ReplyHeadersWidget();

        QString text();

    private:
        Ui::ReplyHeadersWidget* ui;

        // ReplyInspectorWidget interface
    protected:
        void updateReply();
    private slots:
        void on_headersList_customContextMenuRequested(const QPoint& pos);
};

#endif // REPLYHEADERSWIDGET_H
