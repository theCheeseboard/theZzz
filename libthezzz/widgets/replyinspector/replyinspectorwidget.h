#ifndef REPLYINSPECTORWIDGET_H
#define REPLYINSPECTORWIDGET_H

#include "forwarddeclares.h"
#include <QWidget>

struct ReplyInspectorWidgetPrivate;
struct ReplyInspectorWidget : public QWidget {
        Q_OBJECT
    public:
        explicit ReplyInspectorWidget(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~ReplyInspectorWidget();

        virtual QString text() = 0;
        ZzzReplyPtr reply();

    protected:
        virtual void updateReply() = 0;

    private:
        ReplyInspectorWidgetPrivate* d;
};

#endif // REPLYINSPECTORWIDGET_H
