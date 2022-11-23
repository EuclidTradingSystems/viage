#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "qdatetime.h"

#include <json/json.h>

namespace Client
{
QString to_QString(const std::string& str);
std::string to_string(const QString &str);
std::string to_string(const QVariant &var);

QDate to_QDate(const std::string& date, const QString& format = "dd.MM.yyyy");
std::string to_date(const QDate& date, const QString& format = "dd.MM.yyyy");
QDateTime to_QDateTime(const std::string& date, const QString& format = "yyyy-MM-dd hh:mm:ss");
std::string to_date_time(const QDateTime& date, const QString& format = "yyyy-MM-dd hh:mm:ss");

QJsonArray to_QJsonArray(const Json::Value& json);
QJsonObject to_QJsonObject(const Json::Value& json);
QJsonDocument to_QJsonDoc(const Json::Value& json);
Json::Value to_Json(const std::string& str);
Json::Value to_Json(const QByteArray& bytes);
Json::Value to_Json(const QJsonObject& obj);

QByteArray to_QByteArray(const Json::Value& json);
}

#endif // CLIENT
