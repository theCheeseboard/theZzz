#ifndef ZZZREPLYVIEWER_H
#define ZZZREPLYVIEWER_H

#include "forwarddeclares.h"
#include <QWidget>

namespace Ui {
    class ZzzReplyViewer;
}

struct ZzzReplyViewerPrivate;
class ZzzReplyViewer : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzReplyViewer(QWidget* parent = nullptr);
        ~ZzzReplyViewer();

        ZzzReplyManagerPtr replyManager();

    private slots:
        void on_replyList_currentIndexChanged(int index);

    private:
        Ui::ZzzReplyViewer* ui;
        ZzzReplyViewerPrivate* d;
};

#endif // ZZZREPLYVIEWER_H
