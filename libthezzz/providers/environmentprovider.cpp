#include "environmentprovider.h"

#include "widgets/providereditor/environmentprovidereditor.h"
#include "workspacefile.h"
#include <QJsonArray>
#include <ranges/trange.h>

struct EnvironmentProviderPrivate {
        QList<ZzzEnvironment> environments;
        QList<ZzzVariable> variables;
        QList<ZzzEnvironmentVariable> envVars;
        QUuid currentEnvironment;
};

EnvironmentProvider::EnvironmentProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new EnvironmentProviderPrivate();

    auto uuid = QUuid::createUuid();
    d->environments.append({uuid, tr("Production")});
    d->currentEnvironment = uuid;
}

EnvironmentProvider::~EnvironmentProvider() {
    delete d;
}

QList<ZzzEnvironment> EnvironmentProvider::environments() {
    return d->environments;
}

QUuid EnvironmentProvider::addEnvironment(QString name) {
    auto uuid = QUuid::createUuid();
    d->environments.append({uuid, name});
    emit this->workspace()->dataChanged();
    return uuid;
}

void EnvironmentProvider::removeEnvironment(QUuid uuid) {
    if (d->environments.length() <= 1) return;

    d->environments.removeIf([uuid](ZzzEnvironment env) {
        return env.first == uuid;
    });
    emit this->workspace()->dataChanged();
}

QString EnvironmentProvider::environmentName(QUuid environment) {
    return tRange(d->environments).first([environment](ZzzEnvironment env) {
                                      return env.first == environment;
                                  })
        .second;
}

QList<ZzzVariable> EnvironmentProvider::variables() {
    return d->variables;
}

void EnvironmentProvider::setVariables(QList<ZzzVariable> variables) {
    d->variables = variables;
    emit this->workspace()->dataChanged();
}

QList<ZzzEnvironmentVariable> EnvironmentProvider::environmentVariables() {
    return d->envVars;
}

void EnvironmentProvider::setEnvironmentVariables(QList<ZzzEnvironmentVariable> environmentVariables) {
    d->envVars = environmentVariables;
    emit this->workspace()->dataChanged();
}

QUuid EnvironmentProvider::currentEnvironment() {
    return d->currentEnvironment;
}

QString EnvironmentProvider::jsonKey() {
    return QStringLiteral("env");
}

void EnvironmentProvider::loadJson(QJsonValue obj, QJsonValue localObj) {
    d->environments.clear();
    d->variables.clear();
    d->envVars.clear();

    auto jsonObject = obj.toObject();
    auto localObject = localObj.toObject();

    auto environment = jsonObject.value("environment").toArray();
    auto variables = jsonObject.value("variables").toArray();
    auto environmentVariables = jsonObject.value("environmentVariables").toArray();
    auto localEnvironmentVariables = localObject.value("environmentVariables").toArray();

    for (auto env : environment) {
        auto envObj = env.toObject();
        d->environments.append({QUuid::fromString(envObj.value("uuid").toString()), envObj.value("name").toString()});
    }

    for (auto var : variables) {
        auto varObj = var.toObject();
        d->variables.append({QUuid::fromString(varObj.value("uuid").toString()), varObj.value("name").toString(), varObj.value("secret").toBool()});
    }

    for (auto envVar : environmentVariables) {
        auto envVarObj = envVar.toObject();
        d->envVars.append({QUuid::fromString(envVarObj.value("environment").toString()), QUuid::fromString(envVarObj.value("variable").toString()), envVarObj.value("value").toString()});
    }

    for (auto envVar : localEnvironmentVariables) {
        auto envVarObj = envVar.toObject();

        auto envVarEnvironment = QUuid::fromString(envVarObj.value("environment").toString());
        auto envVarVariable = QUuid::fromString(envVarObj.value("variable").toString());

        d->envVars.removeIf([envVarEnvironment, envVarVariable](ZzzEnvironmentVariable envVar) {
            auto [env, var, value] = envVar;
            return env == envVarEnvironment && var == envVarVariable;
        });
        d->envVars.append({envVarEnvironment, envVarVariable, envVarObj.value("value").toString()});
    }

    // Failsafe
    if (d->environments.isEmpty()) d->environments.append({QUuid::createUuid(), tr("Production")});
    d->currentEnvironment = d->environments.first().first;
}

QJsonValue EnvironmentProvider::toJson() {
    QJsonArray environment;
    for (const auto& [uuid, name] : d->environments) {
        environment.append(QJsonObject{
            {"uuid", uuid.toString(QUuid::WithoutBraces)},
            {"name", name                               }
        });
    }

    QJsonArray variables;
    for (const auto& [uuid, name, isSecret] : d->variables) {
        variables.append(QJsonObject{
            {"uuid",   uuid.toString(QUuid::WithoutBraces)},
            {"name",   name                               },
            {"secret", isSecret                           }
        });
    }

    QJsonArray environmentVariables;
    for (const auto& envVar : d->envVars) {
        auto [environment, variable, value] = envVar;
        try {
            auto environmentValue = tRange(d->environments).first([envVar](ZzzEnvironment e) {
                return e.first == std::get<0>(envVar);
            });
            auto variableValue = tRange(d->variables).first([envVar](ZzzVariable v) {
                return std::get<0>(v) == std::get<1>(envVar);
            });

            // Don't save secret variables
            auto savedValue = value;
            if (std::get<2>(variableValue)) {
                savedValue = "";
            }

            environmentVariables.append(QJsonObject{
                {"variable",    variable.toString(QUuid::WithoutBraces)   },
                {"environment", environment.toString(QUuid::WithoutBraces)},
                {"value",       savedValue                                }
            });

        } catch (tRangeException e) {
        }
    }

    return QJsonObject{
        {"environment",          environment         },
        {"variables",            variables           },
        {"environmentVariables", environmentVariables}
    };
}

QList<ProviderEditor*> EnvironmentProvider::editor() {
    return {new EnvironmentProviderEditor(this)};
}

QJsonValue EnvironmentProvider::toLocalJson() {
    QJsonArray environmentVariables;
    for (const auto& envVar : d->envVars) {
        auto [environment, variable, value] = envVar;
        try {
            auto environmentValue = tRange(d->environments).first([envVar](ZzzEnvironment e) {
                return e.first == std::get<0>(envVar);
            });
            auto variableValue = tRange(d->variables).first([envVar](ZzzVariable v) {
                return std::get<0>(v) == std::get<1>(envVar);
            });

            // Only save secret variables
            if (!std::get<2>(variableValue)) continue;

            environmentVariables.append(QJsonObject{
                {"variable",    variable.toString(QUuid::WithoutBraces)   },
                {"environment", environment.toString(QUuid::WithoutBraces)},
                {"value",       value                                     }
            });

        } catch (tRangeException e) {
        }
    }

    return QJsonObject{
        {"environmentVariables", environmentVariables}
    };
}
