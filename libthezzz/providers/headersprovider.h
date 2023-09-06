#ifndef HEADERSPROVIDER_H
#define HEADERSPROVIDER_H

#include "zzzhelpers.h"
#include "zzzprovider.h"
#include <QJsonObject>

struct HeadersProviderPrivate;

class HeadersProvider : public ZzzProvider<HeadersProvider> {
    public:
        HeadersProvider(WorkspaceFile* parent);
        ~HeadersProvider();

        ZzzHeaders headers();
        void setHeaders(ZzzHeaders headers);

    private:
        HeadersProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();

        // ZzzProvider interface
    public:
        QList<ProviderEditor*> editor();
        ZzzHeaders implicitHeaders();
};

#endif // HEADERSPROVIDER_H
