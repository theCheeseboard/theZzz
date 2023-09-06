#ifndef ZZZNETWORKCACHE_H
#define ZZZNETWORKCACHE_H

#include <QAbstractNetworkCache>

class ZzzNetworkCache : public QAbstractNetworkCache {
        Q_OBJECT
    public:
        explicit ZzzNetworkCache(QObject* parent = nullptr);

    signals:

        // QAbstractNetworkCache interface
    public:
        QNetworkCacheMetaData metaData(const QUrl& url);
        void updateMetaData(const QNetworkCacheMetaData& metaData);
        QIODevice* data(const QUrl& url);
        bool remove(const QUrl& url);
        qint64 cacheSize() const;
        QIODevice* prepare(const QNetworkCacheMetaData& metaData);
        void insert(QIODevice* device);

    public slots:
        void clear();
};

#endif // ZZZNETWORKCACHE_H
