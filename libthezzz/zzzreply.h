#ifndef ZZZREPLY_H
#define ZZZREPLY_H

#include "forwarddeclares.h"
#include <QNetworkReply>
#include <QObject>

class QNetworkRequest;
struct ZzzReplyPrivate;
class ZzzReply : public QObject,
                 public ZzzSharedFromThis<ZzzReply> {
        Q_OBJECT
    public:
        explicit ZzzReply(QString verb, QNetworkRequest request, QNetworkReply* reply, QObject* parent = nullptr);
        ~ZzzReply();

        QString verb();
        QUrl url();

        bool finished();
        int statusCode();
        QString reasonPhrase();

        QNetworkReply::NetworkError networkError();
        QString readableNetworkError();

        quint64 requestTime();

        QList<QNetworkReply::RawHeaderPair> headers();
        QByteArray body();

    signals:
        void updated();

    private:
        ZzzReplyPrivate* d;
};

#endif // ZZZREPLY_H
