#ifndef REPLYBODYWIDGET_H
#define REPLYBODYWIDGET_H

#include "forwarddeclares.h"
#include "replyinspectorwidget.h"

namespace Ui {
    class ReplyBodyWidget;
}

struct ReplyBodyWidgetPrivate;
class ReplyBodyWidget : public ReplyInspectorWidget {
        Q_OBJECT

    public:
        explicit ReplyBodyWidget(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~ReplyBodyWidget();

        QString text();

    protected:
        void updateReply();

    private:
        Ui::ReplyBodyWidget* ui;
        ReplyBodyWidgetPrivate* d;
};

#endif // REPLYBODYWIDGET_H
