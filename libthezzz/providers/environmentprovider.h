#ifndef ENVIRONMENTPROVIDER_H
#define ENVIRONMENTPROVIDER_H

#include "zzzprovider.h"
#include <QCoreApplication>

typedef std::pair<QUuid, QString> ZzzEnvironment;
typedef std::tuple<QUuid, QString, bool> ZzzVariable;
typedef std::tuple<QUuid, QUuid, QString> ZzzEnvironmentVariable;

struct EnvironmentProviderPrivate;
class EnvironmentProvider : public ZzzProvider<EnvironmentProvider> {
        Q_DECLARE_TR_FUNCTIONS(EnvironmentProvider)

    public:
        explicit EnvironmentProvider(WorkspaceFile* parent);
        ~EnvironmentProvider();

        QList<ZzzEnvironment> environments();
        QUuid addEnvironment(QString name);
        void removeEnvironment(QUuid uuid);

        QList<ZzzVariable> variables();
        void setVariables(QList<ZzzVariable> variables);

        QList<ZzzEnvironmentVariable> environmentVariables();
        void setEnvironmentVariables(QList<ZzzEnvironmentVariable> environmentVariables);

    private:
        EnvironmentProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();
        QList<ProviderEditor*> editor();
};

#endif // ENVIRONMENTPROVIDER_H
