#ifndef BODYPROVIDER_H
#define BODYPROVIDER_H

#include "zzzprovider.h"

struct BodyProviderPrivate;
class BodyProvider : public ZzzProvider<BodyProvider> {
    public:
        BodyProvider(WorkspaceFile* parent);
        ~BodyProvider();

        QByteArray body();
        void setBody(QByteArray body);

    private:
        BodyProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();
        QList<ProviderEditor*> editor();
};

#endif // BODYPROVIDER_H
