#ifndef JSONBODYWIDGET_H
#define JSONBODYWIDGET_H

#include "replyinspectorwidget.h"
#include <QWidget>

namespace Ui {
    class JsonBodyWidget;
}

class JsonBodyWidget : public ReplyInspectorWidget {
        Q_OBJECT

    public:
        explicit JsonBodyWidget(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~JsonBodyWidget();

        QString text();

    protected:
        void updateReply();

    private:
        Ui::JsonBodyWidget* ui;
};

#endif // JSONBODYWIDGET_H
