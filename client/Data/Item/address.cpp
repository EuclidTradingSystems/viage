#include <iostream>

#include <QVariant>

#include "address.hpp"

namespace Data
{
namespace Places
{
address::address()
    : zip{9000}
    , canton{"Appenzell"}
{
}

QHash<int, QByteArray> address::roleNames()
{
    QHash<int, QByteArray> names;

    names[StreetRole] = "street";
    names[ZipRole] = "zip";
    names[CantonRole] = "canton";
    names[CityRole] = "city";

    return names;
}

QVariant address::data(int role) const
{
    switch (role)
    {
    case StreetRole:
        return street;
    case ZipRole:
        return zip;
    case CantonRole:
        return canton;
    case CityRole:
        return city;
    }

    return QVariant{};
}

void address::setData(const QVariant &value, int role)
{
    switch (role)
    {
    case StreetRole:
        street = value.toString();
        break;
    case ZipRole:
        zip = value.toInt();
        break;
    case CantonRole:
        canton = value.toString();
        break;
    case CityRole:
        city = value.toString();
        break;
    }
}

void address::read(const QJsonObject& json)
{
    if (json.contains("street") && json["street"].isString())
        street = json["street"].toString();

    if (json.contains("zip") && json["zip"].isDouble())
        zip = json["zip"].toInt();

    if (json.contains("canton") && json["canton"].isString())
        canton = json["canton"].toString();

    if (json.contains("city") && json["city"].isString())
        city = json["city"].toString();
}

void address::write(QJsonObject& json) const
{
    json["street"] = street;
    json["zip"] = zip;
    json["canton"] = canton;
    json["city"] = city;
}

bool address::is_completed() const
{
    if (street.isEmpty())
        return false;

    if (city.isEmpty())
        return false;

    return true;
}

}
}
