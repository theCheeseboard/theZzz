#ifndef IDENTIFIERPROVIDER_H
#define IDENTIFIERPROVIDER_H

#include "zzzprovider.h"

struct IdentifierProviderPrivate;
class IdentifierProvider : public ZzzProvider<IdentifierProvider> {
    public:
        explicit IdentifierProvider(WorkspaceFile* parent);
        ~IdentifierProvider();

        QUuid identifier();

    private:
        IdentifierProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();
};

#endif // IDENTIFIERPROVIDER_H
