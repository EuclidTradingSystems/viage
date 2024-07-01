#include <QJsonObject>
#include <QJsonArray>

#include "owner.hpp"
#include "senior_citizen.hpp" // for age Macros

namespace Data
{
namespace People {

owner::owner()
    : civilStatus{None}
    , birthDay{QDate::currentDate().addYears(-AGE_MIN)}
{}

QHash<int, QByteArray> owner::roleNames()
{
    QHash<int, QByteArray> names = infant::roleNames();

    names[BirthDayRole] = "birthDay";
    names[CivilStatusRole] = "civilStatus";
    names[AVSRole] = "avs";

    names.insert(Places::address::roleNames());

    return names;
}

QVariant owner::data(int role) const
{
    QVariant extra_data{infant::data(role)};

    if (extra_data != QVariant{})
        return extra_data;

    switch (role)
    {
    case BirthDayRole:
        return birthDay;
    case CivilStatusRole:
        return civilStatus;
    case AVSRole:
        return avs;
    }

    extra_data = ca.data(role);

    if (extra_data != QVariant{})
        return extra_data;

    return QVariant{};
}

void owner::setData(const QVariant &value, int role)
{
    infant::setData(value, role);

    switch (role)
    {
    case BirthDayRole:
        birthDay = value.toDate();
        break;
    case CivilStatusRole:
        civilStatus = civilStates(value.toInt());
        break;
    case AVSRole:
        avs = value.toString();
        break;
    }

    ca.setData(value, role);
}

void owner::read(const QJsonObject &json)
{
    infant::read(json);

    if (json.contains("birthDay") && json["birthDay"].isString())
        if (json["birthDay"].toString() != "")
            birthDay = QDate::fromString(json["birthDay"].toString(), "dd.MM.yyyy");

    if (json.contains("civilStatus") && json["civilStatus"].isDouble())
        civilStatus = civilStates(json["civilStatus"].toInt());

    if (json.contains("avs") && json["avs"].isString())
        avs = json["avs"].toString();

    if (json.contains("address") && json["address"].isObject())
        ca.read(json["address"].toObject());
}

void owner::write(QJsonObject& json) const
{
    infant::write(json);

    json["birthDay"] = birthDay.toString("dd.MM.yyyy");
    json["civilStatus"] = civilStatus;
    json["avs"] = avs;

    QJsonObject jsonAddress{};
    ca.write(jsonAddress);

    json["address"] = jsonAddress;
}

bool owner::is_completed() const
{
    if (!infant::is_completed())
        return false;

    if (civilStatus == None)
        return false;

    if (avs == "")
        return false;

    if (!ca.is_completed())
        return false;

    return true;
}

}
}
