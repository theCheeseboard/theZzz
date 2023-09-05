#ifndef REPLYBODYWIDGET_H
#define REPLYBODYWIDGET_H

#include "forwarddeclares.h"
#include <QWidget>

namespace Ui {
    class ReplyBodyWidget;
}

struct ReplyBodyWidgetPrivate;
class ReplyBodyWidget : public QWidget {
        Q_OBJECT

    public:
        explicit ReplyBodyWidget(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~ReplyBodyWidget();

    private:
        Ui::ReplyBodyWidget* ui;
        ReplyBodyWidgetPrivate* d;

        void updateReply();
};

#endif // REPLYBODYWIDGET_H
