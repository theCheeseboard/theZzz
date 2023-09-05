#ifndef ZZZREPLYINSPECTOR_H
#define ZZZREPLYINSPECTOR_H

#include "forwarddeclares.h"
#include <QWidget>

namespace Ui {
    class ZzzReplyInspector;
}

struct ZzzReplyInspectorPrivate;
class ZzzReplyInspector : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzReplyInspector(ZzzReplyPtr reply, QWidget* parent = nullptr);
        ~ZzzReplyInspector();

    private:
        Ui::ZzzReplyInspector* ui;
        ZzzReplyInspectorPrivate* d;
};

#endif // ZZZREPLYINSPECTOR_H
