#include "zzzreplymanager.h"

#include "zzzreply.h"

struct ZzzReplyManagerPrivate {
        QList<ZzzReplyPtr> replies;
};

ZzzReplyManager::ZzzReplyManager(QObject* parent) :
    QObject{parent} {
    d = new ZzzReplyManagerPrivate();
}

ZzzReplyManager::~ZzzReplyManager() {
    delete d;
}

void ZzzReplyManager::pushReply(ZzzReplyPtr reply) {
    d->replies.append(reply);
    emit newReply(reply);
}

void ZzzReplyManager::clear() {
    d->replies.clear();
    emit repliesCleared();
}

QList<ZzzReplyPtr> ZzzReplyManager::replies() {
    return d->replies;
}
