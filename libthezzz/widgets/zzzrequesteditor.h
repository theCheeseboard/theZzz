#ifndef ZZZREQUESTEDITOR_H
#define ZZZREQUESTEDITOR_H

#include "forwarddeclares.h"
#include "libthezzz_global.h"
#include <QWidget>

namespace Ui {
    class ZzzRequestEditor;
}

struct ZzzRequestEditorPrivate;
class LIBTHEZZZ_EXPORT ZzzRequestEditor : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzRequestEditor(ZzzRequestTreeItemPtr request, QWidget* parent = nullptr);
        ~ZzzRequestEditor();

        void executeRequest();

    signals:
        void addReply(ZzzReplyPtr reply);

    private:
        Ui::ZzzRequestEditor* ui;
        ZzzRequestEditorPrivate* d;
};

#endif // ZZZREQUESTEDITOR_H
