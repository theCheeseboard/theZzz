#ifndef FORWARDDECLARES_H
#define FORWARDDECLARES_H

#include <QSharedPointer>

#define FORWARD_DECLARE(x) \
    class x;               \
    typedef QSharedPointer<x> x##Ptr;

FORWARD_DECLARE(WorkspaceFile)
FORWARD_DECLARE(ZzzReply)
FORWARD_DECLARE(ZzzRequest)
FORWARD_DECLARE(ZzzRequestTreeItem)
FORWARD_DECLARE(ZzzReplyManager)

FORWARD_DECLARE(RequestContainerProvider)
FORWARD_DECLARE(EndpointProvider)

FORWARD_DECLARE(ProviderEditor)

#undef FORWARD_DECLARE

template<typename T> class ZzzSharedFromThis : public QEnableSharedFromThis<T> {
    public:
        QSharedPointer<T> sharedFromThis() {
            auto ptr = QEnableSharedFromThis<T>::sharedFromThis();
            if (ptr) return ptr;
            return QSharedPointer<T>(static_cast<T*>(this));
        }
};

#endif // FORWARDDECLARES_H
