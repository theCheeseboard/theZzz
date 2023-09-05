#include "zzzreply.h"

#include <QNetworkReply>

struct ZzzReplyPrivate {
        QString verb;
        QNetworkRequest request;
        QNetworkReply* reply;

        QByteArray body;
};

ZzzReply::ZzzReply(QString verb, QNetworkRequest request, QNetworkReply* reply, QObject* parent) :
    QObject{parent} {
    d = new ZzzReplyPrivate();
    d->verb = verb;
    d->request = request;
    d->reply = reply;

    connect(reply, &QNetworkReply::readyRead, this, [reply, this] {
        d->body.append(reply->readAll());
        emit updated();
    });
}

ZzzReply::~ZzzReply() {
    delete d;
}

QString ZzzReply::verb() {
    return d->verb;
}

QUrl ZzzReply::url() {
    return d->reply->url();
}

QByteArray ZzzReply::body() {
    return d->body;
}
