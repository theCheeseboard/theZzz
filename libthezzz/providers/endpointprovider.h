#ifndef ENDPOINTPROVIDER_H
#define ENDPOINTPROVIDER_H

#include "environmentprovider.h"
#include "zzzprovider.h"
#include <QString>

struct EndpointProviderPrivate;

class EndpointProvider : public ZzzProvider<EndpointProvider> {
    public:
        EndpointProvider(WorkspaceFile* parent);
        ~EndpointProvider();

        QString verb();
        void setVerb(QString verb);

        QString endpoint();
        void setEndpoint(QString endpoint);

        QUrl calculateUrl(QList<ZzzVariable>* missingEnvironmentVariables = nullptr);

    private:
        EndpointProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj, QJsonValue localObj);
        QJsonValue toJson();
        ZzzHeaders implicitHeaders();
};

#endif // ENDPOINTPROVIDER_H
