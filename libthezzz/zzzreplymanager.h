#ifndef ZZZREPLYMANAGER_H
#define ZZZREPLYMANAGER_H

#include "forwarddeclares.h"
#include <QObject>

struct ZzzReplyManagerPrivate;
class ZzzReplyManager : public QObject,
                        public ZzzSharedFromThis<ZzzReplyManager> {
        Q_OBJECT
    public:
        explicit ZzzReplyManager(QObject* parent = nullptr);
        ~ZzzReplyManager();

        void pushReply(ZzzReplyPtr reply);
        void clear();
        QList<ZzzReplyPtr> replies();

    signals:
        void newReply(ZzzReplyPtr reply);
        void repliesCleared();

    private:
        ZzzReplyManagerPrivate* d;
};

#endif // ZZZREPLYMANAGER_H
