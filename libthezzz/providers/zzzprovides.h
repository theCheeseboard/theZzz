#ifndef ZZZPROVIDES_H
#define ZZZPROVIDES_H

#include "zzzprovider.h"

// template<typename T>
// concept ProviderClass = std::derived_from<T, ZzzProvider>;

QJsonObject operator<<(const QJsonObject& first, const QJsonObject& second);

template<typename... Providers>
class ZzzProvides : public Providers... {
    public:
        ZzzProvides(WorkspaceFile* parent) :
            Providers(parent)... {
        }

        virtual void loadJson(QJsonObject obj) {
            (Providers::loadJsonProvider(obj), ...);
        }

        virtual QJsonObject toJson() {
            return (... << Providers::saveJsonProvider());
        }
};

#endif // ZZZPROVIDES_H
