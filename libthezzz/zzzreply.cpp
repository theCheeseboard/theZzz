#include "zzzreply.h"

#include <QElapsedTimer>
#include <QNetworkReply>

struct ZzzReplyPrivate {
        QString verb;
        QNetworkRequest request;
        QNetworkReply* reply;

        QByteArray body;
        bool finished = false;

        QElapsedTimer elapsedTimer;
        quint64 msecsRequestTime;
        static QMap<int, QString> reasonPhraseMap;
};

QMap<int, QString> ZzzReplyPrivate::reasonPhraseMap = {
    {100, QStringLiteral("Continue")                     },
    {101, QStringLiteral("Switching Protocols")          },
    {200, QStringLiteral("OK")                           },
    {201, QStringLiteral("Created")                      },
    {202, QStringLiteral("Accepted")                     },
    {203, QStringLiteral("Non-Authoritative Information")},
    {204, QStringLiteral("No Content")                   },
    {205, QStringLiteral("Reset Content")                },
    {206, QStringLiteral("Partial Content")              },
    {300, QStringLiteral("Multiple Choices")             },
    {301, QStringLiteral("Moved Permanently")            },
    {302, QStringLiteral("Found")                        },
    {303, QStringLiteral("See Other")                    },
    {304, QStringLiteral("Not Modified")                 },
    {305, QStringLiteral("Use Proxy")                    },
    {307, QStringLiteral("Temporary Redirect")           },
    {400, QStringLiteral("Bad Request")                  },
    {401, QStringLiteral("Unauthorized")                 },
    {402, QStringLiteral("Payment Required")             },
    {403, QStringLiteral("Forbidden")                    },
    {404, QStringLiteral("Not Found")                    },
    {405, QStringLiteral("Method Not Allowed")           },
    {406, QStringLiteral("Not Acceptable")               },
    {407, QStringLiteral("Proxy Authentication Required")},
    {408, QStringLiteral("Request Timeout")              },
    {409, QStringLiteral("Conflict")                     },
    {410, QStringLiteral("Gone")                         },
    {411, QStringLiteral("Length Required")              },
    {412, QStringLiteral("Precondition Failed")          },
    {413, QStringLiteral("Payload Too Large")            },
    {414, QStringLiteral("URI Too Long")                 },
    {415, QStringLiteral("Unsupported Media Type")       },
    {416, QStringLiteral("Range Not Satisfiable")        },
    {417, QStringLiteral("Expectation Failed")           },
    {500, QStringLiteral("Internal Server Error")        },
    {501, QStringLiteral("Not Implemented")              },
    {502, QStringLiteral("Bad Gateway")                  },
    {503, QStringLiteral("Service Unavailable")          },
    {504, QStringLiteral("Gateway Timeout")              },
    {505, QStringLiteral("HTTP Version Not Supported")   }
};

ZzzReply::ZzzReply(QString verb, QNetworkRequest request, QNetworkReply* reply, QObject* parent) :
    QObject{parent} {
    d = new ZzzReplyPrivate();
    d->verb = verb;
    d->request = request;
    d->reply = reply;
    d->elapsedTimer.start();

    connect(reply, &QNetworkReply::readyRead, this, [reply, this] {
        d->body.append(reply->readAll());
        emit updated();
    });
    connect(reply, &QNetworkReply::finished, this, [this] {
        d->finished = true;
        d->msecsRequestTime = d->elapsedTimer.nsecsElapsed() / 1000000;
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

bool ZzzReply::finished() {
    return d->finished;
}

int ZzzReply::statusCode() {
    return d->reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

QString ZzzReply::reasonPhrase() {
    auto reasonPhrase = d->reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
    if (!reasonPhrase.isEmpty()) return reasonPhrase;

    auto statusCode = this->statusCode();
    if (d->reasonPhraseMap.contains(statusCode)) return d->reasonPhraseMap.value(statusCode);

    if (statusCode < 200) {
        return QStringLiteral("Informational");
    } else if (statusCode < 300) {
        return QStringLiteral("Successful");
    } else if (statusCode < 400) {
        return QStringLiteral("Redirection");
    } else if (statusCode < 500) {
        return QStringLiteral("Client Error");
    } else if (statusCode < 600) {
        return QStringLiteral("Server Error");
    } else {
        return QStringLiteral("");
    }
}

quint64 ZzzReply::requestTime() {
    return d->msecsRequestTime;
}

QList<QNetworkReply::RawHeaderPair> ZzzReply::headers() {
    return d->reply->rawHeaderPairs();
}

QByteArray ZzzReply::body() {
    return d->body;
}
