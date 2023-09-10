#ifndef ZZZPROVIDER_H
#define ZZZPROVIDER_H

#include "forwarddeclares.h"
#include "zzzhelpers.h"
#include <QJsonObject>

class ZzzProviderBase {
};

struct ZzzProviderPrivate;
template<typename T>
class ZzzProvider : public ZzzProviderBase {
    public:
        ZzzProvider(WorkspaceFile* parent) {
            _parent = parent;
        }
        ~ZzzProvider() {
        }

        WorkspaceFile* workspace() {
            return _parent;
        }

        virtual QString jsonKey() = 0;
        virtual void loadJson(QJsonValue obj, QJsonValue localObj) = 0;
        virtual QJsonValue toJson() = 0;
        virtual QJsonValue toLocalJson() {
            return {};
        }

        virtual QList<ProviderEditor*> editor() {
            return {};
        }

        virtual ZzzHeaders implicitHeaders() {
            return {};
        }

    protected:
        void loadJsonProvider(QJsonObject obj, QJsonObject localObj) {
            dynamic_cast<T*>(this)->T::loadJson(obj.value(this->jsonKey()), localObj.value(this->jsonKey()));
        }
        QJsonObject saveJsonProvider() {
            return {
                {dynamic_cast<T*>(this)->T::jsonKey(), dynamic_cast<T*>(this)->T::toJson()}
            };
        }
        QJsonObject saveLocalJsonProvider() {
            return {
                {dynamic_cast<T*>(this)->T::jsonKey(), dynamic_cast<T*>(this)->T::toLocalJson()}
            };
        }

    private:
        WorkspaceFile* _parent;
};

#endif // ZZZPROVIDER_H
