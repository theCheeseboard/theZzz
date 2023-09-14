#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include "libthezzz_global.h"
#include "zzzprovider.h"
#include <QObject>

struct SettingsProviderPrivate;
class LIBTHEZZZ_EXPORT SettingsProvider : public ZzzProvider<SettingsProvider> {
    public:
        explicit SettingsProvider(WorkspaceFile* parent);
        ~SettingsProvider();

        void setSkipTlsVerification(bool skipTlsVerification);
        bool skipTlsVerification();

    signals:

    private:
        SettingsProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj, QJsonValue localObj);
        QJsonValue toJson();
        QList<ProviderEditor*> editor();
};

#endif // SETTINGSPROVIDER_H
