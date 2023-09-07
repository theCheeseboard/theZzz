#ifndef ZZZREPLY_H
#define ZZZREPLY_H

#include "forwarddeclares.h"
#include <QNetworkReply>
#include <QObject>
#include "libthezzz_global.h"

class QNetworkRequest;
struct ZzzReplyPrivate;
class LIBTHEZZZ_EXPORT ZzzReply : public QObject,
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
