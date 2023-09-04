#ifndef ENDPOINTPROVIDER_H
#define ENDPOINTPROVIDER_H

#include <QString>

struct EndpointProviderPrivate;

class EndpointProvider {
    public:
        EndpointProvider();
        ~EndpointProvider();

        QString verb();
        void setVerb(QString verb);

        QString endpoint();
        void setEndpoint(QString endpoint);

    private:
        EndpointProviderPrivate* d;
};

#endif // ENDPOINTPROVIDER_H
