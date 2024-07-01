#pragma once
#include <QVariant>

#include "c_person.hpp"

namespace Data
{
namespace People
{
c_person::c_person()
    : phone{"+41"}
{}

QHash<int, QByteArray> c_person::roleNames()
{
    QHash<int, QByteArray> names;

    names[IdRole] = "id";
    names[FirstNameRole] = "firstName";
    names[LastNameRole] = "lastName";
    names[PhoneRole] = "phone";
    names[EmailRole] = "eMail";

    return names;
}

QVariant c_person::data(int role) const
{
    switch (role)
    {
    case IdRole:
        return id;
    case FirstNameRole:
        return firstName;
    case LastNameRole:
        return lastName;
    case PhoneRole:
        return phone;
    case EmailRole:
        return eMail;
    }

    return QVariant{};
}

void c_person::setData(const QVariant& value, int role)
{
    switch (role)
    {
    case FirstNameRole:
        firstName = value.toString();
        break;
    case LastNameRole:
        lastName = value.toString();
        break;
    case PhoneRole:
        phone = value.toString();
        break;
    case EmailRole:
        eMail = value.toString();
        break;
    }
}

void c_person::read(const QJsonObject& json)
{
    if (json.contains("id") && json["id"].isDouble())
        id = json["id"].toInt();

    if (json.contains("firstName") && json["firstName"].isString())
        firstName = json["firstName"].toString();

    if (json.contains("lastName") && json["lastName"].isString())
        lastName = json["lastName"].toString();

    if (json.contains("phone") && json["phone"].isString())
        phone = json["phone"].toString();

    if (json.contains("eMail") && json["eMail"].isString())
        eMail = json["eMail"].toString();
}

void c_person::write(QJsonObject& json) const
{
    json["id"] = id;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["phone"] = phone;
    json["eMail"] = eMail;
}

}
}
