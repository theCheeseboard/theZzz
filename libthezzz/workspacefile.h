#ifndef WORKSPACEFILE_H
#define WORKSPACEFILE_H

#include "forwarddeclares.h"
#include "providers/headersprovider.h"
#include "providers/requestcontainerprovider.h"
#include <QObject>

class QNetworkAccessManager;
struct WorkspaceFilePrivate;
class WorkspaceFile : public QObject,
                      public ZzzSharedFromThis<WorkspaceFile>,
                      public HeadersProvider,
                      public RequestContainerProvider {
        Q_OBJECT
    public:
        explicit WorkspaceFile(QObject* parent = nullptr);
        ~WorkspaceFile();

        void loadJson(QJsonObject object);
        QJsonObject toJson();

        QNetworkAccessManager* networkAccessManager();

    signals:
        void requestsChanged();

    private:
        WorkspaceFilePrivate* d;
};

typedef QSharedPointer<WorkspaceFile> WorkspaceFilePtr;

#endif // WORKSPACEFILE_H
