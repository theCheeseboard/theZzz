#ifndef REQUESTCONTAINERPROVIDER_H
#define REQUESTCONTAINERPROVIDER_H

#include "forwarddeclares.h"
#include "zzzprovider.h"
#include "zzzrequesttreeitem.h"

struct RequestContainerProviderPrivate;

class WorkspaceFile;
class RequestContainerProvider : public ZzzProvider<RequestContainerProvider> {
    public:
        RequestContainerProvider(WorkspaceFile* parent);
        ~RequestContainerProvider();

        void addRequest(ZzzRequestTreeItemPtr request);
        QList<ZzzRequestTreeItemPtr> requests();

        template<typename T> QList<T*> ancestors(ZzzProvidesBase* item, bool* isChild = nullptr) {
            for (const auto& request : this->requests()) {
                if (request.dynamicCast<ZzzProvidesBase>() == item) {
                    // Found the item
                    if (isChild) *isChild = true;

                    if (auto casted = dynamic_cast<T*>(this)) {
                        return {casted};
                    }

                    return {};
                }

                if (auto container = request.dynamicCast<RequestContainerProvider>()) {
                    bool isChildChild = false;
                    auto response = container->ancestors<T>(item, &isChildChild);
                    if (isChildChild) {
                        if (auto casted = dynamic_cast<T*>(this)) {
                            response.prepend(casted);
                        }
                        return response;
                    }
                }
            }

            if (isChild) *isChild = false;
            return {};
        }

        template<typename T> QList<T*> ancestorsAndSelf(ZzzProvidesBase* item) {
            QList<T*> response;
            response.append(this->ancestors<T>(item));
            if (auto casted = dynamic_cast<T*>(item)) {
                response.append(casted);
            }
            return response;
        }

    private:
        RequestContainerProviderPrivate* d;

        // ZzzProvider interface
    public:
        QString jsonKey();
        void loadJson(QJsonValue obj);
        QJsonValue toJson();
};

#endif // REQUESTCONTAINERPROVIDER_H
