#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

#include <wobjectimpl.h>

#include "netManager.hpp"

namespace Interface
{
W_OBJECT_IMPL(netManager)

netManager::netManager(const QString& url,
                       const QString &authentication_arguments,
                       const QString &extra_arguments)
    : prefix{url + '/'}
    , auth_args{authentication_arguments}
    , suffix{extra_arguments}
    , rqst{}
{
    auto conf = QSslConfiguration::defaultConfiguration();
    rqst.setSslConfiguration(conf);
    rqst.setHeader(QNetworkRequest::ContentTypeHeader,
                   "application/json");

    setTransferTimeout();

    connect(this, &QNetworkAccessManager::sslErrors,
            this, [](QNetworkReply* reply,
            const QList<QSslError>& errors)
    { reply->ignoreSslErrors(errors); });

    connect(this, &QNetworkAccessManager::finished,
            this, [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
            emit replyError("netManager reply error",
                            reply->errorString());
    });
}

void netManager::authenticate(const QString& username,
                              const QString& password)
{
    QUrl url{prefix + auth_args
                + '?' + "userName=" + username
                + '&' + "password=" + password
                + '&' + suffix
                + "&rememberMe=True"};

    rqst.setUrl(url);

    authenticating = true;
    auto* reply = get(rqst);

    connect(reply, &QNetworkReply::finished,
            [this, reply]()
    {
        if (reply->error())
        {
            if (authenticating)
            {
                authenticating = false;
                emit loggedIn(false, reply->errorString());
            }
        }
        else
        {
            const auto res = reply->readAll();
            const auto json = QJsonDocument::fromJson(res).object();

            if (json.contains("sessionId"))
            {
                if (authenticating)
                {
                    authenticating = false;

                    if (json.contains("sessionId") && json["sessionId"].isString())
                    {
                        const auto str{json["sessionId"].toString()};
                        rqst.setRawHeader("sessionId",
                                          QByteArray::fromStdString(str.toStdString()));
                    }

                    if (json.contains("id") && json["id"].isDouble())
                        emit userChanged(json["id"].toInt());

                    if (json.contains("clearance") && json["clearance"].isDouble())
                        emit clearanceChanged(json["clearance"].toInt());

                    emit loggedIn(true);
                }
            }
        }

        reply->deleteLater();
    });
}

void netManager::downloadFile(const char* key,
                              const QString &path,
                              const std::function<void (bool, const QString &)> &callback)
{
    setRequest(key);
    auto* reply = get(rqst);
    setCallback(reply,
                [path, callback](const QByteArray& bytes)
    {
        QSaveFile file(path);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(bytes);
            if (file.commit())
                callback(true, "");
            else
                callback(false, file.errorString());
        }
        else
            callback(false, file.errorString());
    });
}

void netManager::getFromKey(const char* key,
                            const std::function<void (const QByteArray &)> &callback)
{
    setRequest(key);
    auto* reply = get(rqst);
    setCallback(reply, callback);
}

void netManager::putToKey(const char* key,
                          const QByteArray &data,
                          const std::function<void (const QJsonObject &)> &callback,
                          const QString& errorPrefix)
{
    setRequest(key);
    auto* reply = put(rqst, data);
    setCallback(reply, callback, errorPrefix);
}

void netManager::postToKey(const char* key,
                           const QByteArray &data,
                           const std::function<void (const QJsonObject &)> &callback,
                           const QString& errorPrefix)
{
    setRequest(key);
    auto* reply = post(rqst, data);
    setCallback(reply, callback, errorPrefix);
}

void netManager::deleteToKey(const char *key,
                             const QByteArray &data,
                             const std::function<void (const QJsonObject &)> &callback,
                             const QString& errorPrefix)
{
    setRequest(key);
    auto* reply = sendCustomRequest(rqst, "DELETE", data);
    setCallback(reply, callback, errorPrefix);
}

void netManager::setCallback(QNetworkReply* reply,
                             const std::function<void (const QJsonObject &)> &callback,
                             const QString& errorPrefix)
{
    connect(reply, &QNetworkReply::finished,
            this,
            [reply, callback, errorPrefix, this]()
    {
        const auto json{QJsonDocument::fromJson(reply->readAll()).object()};
        if (json.contains("success"))
        {
            if (json["success"].toBool())
                callback(json);
            else
                emit replyError(errorPrefix, json["errorMessage"].toString());
        }
        else
            emit replyError(errorPrefix);

        reply->deleteLater();
    });
}

void netManager::setCallback(QNetworkReply* reply,
                             const std::function<void (const QByteArray &)> &callback)
{
    connect(reply, &QNetworkReply::finished,
            [reply, callback]()
    {
        callback(reply->readAll());
        reply->deleteLater();
    });
}

void netManager::setRequest(const char* key)
{
    QUrl url{prefix + key + '?' + suffix};
    rqst.setUrl(url);
}

}
