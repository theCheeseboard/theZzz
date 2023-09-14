#include "settingsprovider.h"

#include "widgets/providereditor/settingsprovidereditor.h"
#include "workspacefile.h"

struct SettingsProviderPrivate {
        bool skipTlsVerification;
};

SettingsProvider::SettingsProvider(WorkspaceFile* parent) :
    ZzzProvider{parent} {
    d = new SettingsProviderPrivate();
}

SettingsProvider::~SettingsProvider() {
    delete d;
}

void SettingsProvider::setSkipTlsVerification(bool skipTlsVerification) {
    d->skipTlsVerification = skipTlsVerification;
    emit this->workspace()->dataChanged();
}

bool SettingsProvider::skipTlsVerification() {
    return d->skipTlsVerification;
}

QString SettingsProvider::jsonKey() {
    return QStringLiteral("settings");
}

void SettingsProvider::loadJson(QJsonValue obj, QJsonValue localObj) {
    auto jsonObj = obj.toObject();
    d->skipTlsVerification = jsonObj.value("skipTlsVerification").toBool();
}

QJsonValue SettingsProvider::toJson() {
    return QJsonObject{
        {"skipTlsVerification", d->skipTlsVerification}
    };
}

QList<ProviderEditor*> SettingsProvider::editor() {
    return {new SettingsProviderEditor(this)};
}
