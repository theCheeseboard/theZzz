#ifndef ZZZREQUESTEDITOR_H
#define ZZZREQUESTEDITOR_H

#include "forwarddeclares.h"
#include <QWidget>

namespace Ui {
    class ZzzRequestEditor;
}

struct ZzzRequestEditorPrivate;
class ZzzRequestEditor : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzRequestEditor(QSharedPointer<QObject> request, QWidget* parent = nullptr);
        ~ZzzRequestEditor();

        void executeRequest();

    signals:
        void addReply(ZzzReplyPtr reply);

    private:
        Ui::ZzzRequestEditor* ui;
        ZzzRequestEditorPrivate* d;
};

#endif // ZZZREQUESTEDITOR_H
