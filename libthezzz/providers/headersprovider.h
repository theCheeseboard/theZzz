#ifndef HEADERSPROVIDER_H
#define HEADERSPROVIDER_H

#include <QJsonObject>

struct HeadersProviderPrivate;

class HeadersProvider {
    public:
        HeadersProvider();
        ~HeadersProvider();

    protected:
        void loadHeadersJson(QJsonObject obj);
        QJsonObject headersToJson();

    private:
        HeadersProviderPrivate* d;
};

#endif // HEADERSPROVIDER_H
