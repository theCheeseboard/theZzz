#ifndef ENVIRONMENTPROVIDER_H
#define ENVIRONMENTPROVIDER_H

#include "libthezzz_global.h"
#include "zzzprovider.h"
#include <QCoreApplication>
#include <texception.h>

typedef std::pair<QUuid, QString> ZzzEnvironment;
typedef std::tuple<QUuid, QString, bool> ZzzVariable;
typedef std::tuple<QUuid, QUuid, QString> ZzzEnvironmentVariable;

struct EnvironmentProviderPrivate;
class LIBTHEZZZ_EXPORT EnvironmentProvider : public ZzzProvider<EnvironmentProvider> {
        Q_DECLARE_TR_FUNCTIONS(EnvironmentProvider)

    public:
        explicit EnvironmentProvider(WorkspaceFile* parent);
        ~EnvironmentProvider();

        QList<ZzzEnvironment> environments();
        QUuid addEnvironment(QString name);
        void removeEnvironment(QUuid uuid);
        QString environmentName(QUuid environment);

        QList<ZzzVariable> variables();
        void setVariables(QList<ZzzVariable> variables);
        QString variableName(QUuid variable);

        QList<ZzzEnvironmentVariable> environmentVariables();
        void setEnvironmentVariables(QList<ZzzEnvironmentVariable> environmentVariables);

        QUuid currentEnvironment();
        void setCurrentEnvironment(QUuid environment);
        QString substituteEnvironment(QString string, QList<ZzzVariable>* missingEnvironmentVariables = nullptr);

    private:
        EnvironmentProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj, QJsonValue localObj);
        QJsonValue toJson();
        QJsonValue toLocalJson();
        QList<ProviderEditor*> editor();
};

#endif // ENVIRONMENTPROVIDER_H
