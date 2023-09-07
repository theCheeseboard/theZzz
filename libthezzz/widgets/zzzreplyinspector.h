#ifndef ZZZREPLYINSPECTOR_H
#define ZZZREPLYINSPECTOR_H

#include "forwarddeclares.h"
#include "libthezzz_global.h"
#include <QWidget>

namespace Ui {
    class ZzzReplyInspector;
}

class ReplyInspectorWidget;
struct ZzzReplyInspectorPrivate;
class LIBTHEZZZ_EXPORT ZzzReplyInspector : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzReplyInspector(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~ZzzReplyInspector();

    private:
        Ui::ZzzReplyInspector* ui;
        ZzzReplyInspectorPrivate* d;

        void updateReply();
        void addReplyInspectorWidget(ReplyInspectorWidget* widget);
};

#endif // ZZZREPLYINSPECTOR_H
