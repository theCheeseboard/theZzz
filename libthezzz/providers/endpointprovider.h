#ifndef ENDPOINTPROVIDER_H
#define ENDPOINTPROVIDER_H

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

    private:
        EndpointProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();
};

#endif // ENDPOINTPROVIDER_H
