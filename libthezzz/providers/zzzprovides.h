#ifndef ZZZPROVIDES_H
#define ZZZPROVIDES_H

#include "libthezzz_global.h"
#include "zzzhelpers.h"
#include "zzzprovider.h"

// template<typename T>
// concept ProviderClass = std::derived_from<T, ZzzProvider>;

QJsonObject operator<<(const QJsonObject& first, const QJsonObject& second);

class LIBTHEZZZ_EXPORT ZzzProvidesBase {
    public:
        virtual QList<ProviderEditor*> editors() = 0;
        virtual ZzzHeaders allImplicitHeaders() = 0;
};

template<typename... Providers>
class LIBTHEZZZ_EXPORT ZzzProvides : public ZzzProvidesBase,
                                     public Providers... {
    public:
        ZzzProvides(WorkspaceFile* parent) :
            Providers(parent)... {
        }

        virtual void loadJson(QJsonObject obj, QJsonObject localObj) {
            (Providers::loadJsonProvider(obj, localObj), ...);
        }

        virtual QJsonValue toJson() {
            return QJsonObject((... << Providers::saveJsonProvider()));
        }

        virtual QJsonValue toLocalJson() {
            return QJsonObject((... << Providers::saveLocalJsonProvider()));
        }

        virtual QList<ProviderEditor*> editors() {
            QList<ProviderEditor*> editor;
            (editor << ... << Providers::editor());
            editor.removeAll(nullptr);
            return editor;
        }

        virtual ZzzHeaders allImplicitHeaders() {
            ZzzHeaders headers;
            (headers << ... << Providers::implicitHeaders());
            return headers;
        }
};

#endif // ZZZPROVIDES_H
