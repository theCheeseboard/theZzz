#ifndef WORKSPACEFILE_H
#define WORKSPACEFILE_H

#include "forwarddeclares.h"
#include "libthezzz_global.h"
#include "providers/headersprovider.h"
#include "providers/requestcontainerprovider.h"
#include "providers/zzzprovides.h"
#include <QObject>

typedef ZzzProvides<HeadersProvider, RequestContainerProvider> WorkspaceFileZzzProvides;

class QNetworkAccessManager;
struct WorkspaceFilePrivate;
class LIBTHEZZZ_EXPORT WorkspaceFile : public QObject,
                                       public ZzzSharedFromThis<WorkspaceFile>,
                                       public WorkspaceFileZzzProvides {
        Q_OBJECT
    public:
        explicit WorkspaceFile(QObject* parent = nullptr);
        ~WorkspaceFile();

        void loadJson(QJsonValue object);
        QJsonValue toJson();

        QNetworkAccessManager* networkAccessManager();

    signals:
        void requestsChanged();
        void dataChanged();

    private:
        WorkspaceFilePrivate* d;
};

typedef QSharedPointer<WorkspaceFile> WorkspaceFilePtr;

#endif // WORKSPACEFILE_H
