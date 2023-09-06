#ifndef HEADERSPROVIDER_H
#define HEADERSPROVIDER_H

#include "zzzprovider.h"
#include <QJsonObject>

struct HeadersProviderPrivate;

class HeadersProvider : public ZzzProvider<HeadersProvider> {
    public:
        HeadersProvider(WorkspaceFile* parent);
        ~HeadersProvider();

    private:
        HeadersProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();
};

#endif // HEADERSPROVIDER_H
