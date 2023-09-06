#ifndef ZZZPROVIDES_H
#define ZZZPROVIDES_H

#include "zzzprovider.h"

// template<typename T>
// concept ProviderClass = std::derived_from<T, ZzzProvider>;

QJsonObject operator<<(const QJsonObject& first, const QJsonObject& second);

class ZzzProvidesBase {
    public:
        virtual QList<ProviderEditor*> editors() = 0;
};

template<typename... Providers>
class ZzzProvides : public ZzzProvidesBase,
                    public Providers... {
    public:
        ZzzProvides(WorkspaceFile* parent) :
            Providers(parent)... {
        }

        virtual void loadJson(QJsonObject obj) {
            (Providers::loadJsonProvider(obj), ...);
        }

        virtual QJsonValue toJson() {
            return QJsonObject((... << Providers::saveJsonProvider()));
        }

        virtual QList<ProviderEditor*> editors() {
            QList<ProviderEditor*> editor;
            (editor << ... << Providers::editor());
            editor.removeAll(nullptr);
            return editor;
        }
};

#endif // ZZZPROVIDES_H
