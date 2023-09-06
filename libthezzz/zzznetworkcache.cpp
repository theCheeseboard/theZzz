#include "zzznetworkcache.h"

ZzzNetworkCache::ZzzNetworkCache(QObject* parent) :
    QAbstractNetworkCache{parent} {
}

QNetworkCacheMetaData ZzzNetworkCache::metaData(const QUrl& url) {
    return {};
}

void ZzzNetworkCache::updateMetaData(const QNetworkCacheMetaData& metaData) {
}

QIODevice* ZzzNetworkCache::data(const QUrl& url) {
    return nullptr;
}

bool ZzzNetworkCache::remove(const QUrl& url) {
    return true;
}

qint64 ZzzNetworkCache::cacheSize() const {
    return 0;
}

QIODevice* ZzzNetworkCache::prepare(const QNetworkCacheMetaData& metaData) {
    return nullptr;
}

void ZzzNetworkCache::insert(QIODevice* device) {
}

void ZzzNetworkCache::clear() {
}
