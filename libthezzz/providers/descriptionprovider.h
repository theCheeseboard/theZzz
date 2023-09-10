#ifndef DESCRIPTIONPROVIDER_H
#define DESCRIPTIONPROVIDER_H

#include "zzzprovider.h"

struct DescriptionProviderPrivate;
class DescriptionProvider : public ZzzProvider<DescriptionProvider> {
    public:
        DescriptionProvider(WorkspaceFile* parent);
        ~DescriptionProvider();

        QString title();
        void setTitle(QString title);

        QString description();
        void setDescription(QString description);

    private:
        DescriptionProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj, QJsonValue localObj);
        QJsonValue toJson();
        QJsonValue toLocalJson();
        QList<ProviderEditor*> editor();
};

#endif // DESCRIPTIONPROVIDER_H
