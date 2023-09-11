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

        QString contentType();
        void setContentType(QString contentType);

    private:
        BodyProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj, QJsonValue localObj);
        QJsonValue toJson();
        QList<ProviderEditor*> editor();
        ZzzHeaders implicitHeaders();
};

#endif // BODYPROVIDER_H
