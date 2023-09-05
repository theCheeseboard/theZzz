#include "zzzprovides.h"

QJsonObject operator<<(const QJsonObject& first, const QJsonObject& second) {
    QJsonObject combined;
    for (const auto& key : first.keys()) {
        combined.insert(key, first.value(key));
    }
    for (const auto& key : second.keys()) {
        combined.insert(key, second.value(key));
    }
    return combined;
}
