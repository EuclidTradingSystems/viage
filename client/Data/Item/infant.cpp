#pragma once
#include <QVariant>

#include "c_infant.hpp"

namespace Data
{
namespace People
{
QHash<int, QByteArray> c_infant::roleNames()
{
    QHash<int, QByteArray> names = c_person::roleNames();

    names[SexRole] = "sex";

    return names;
}

QVariant c_infant::data(int role) const
{
    QVariant base_data{c_person::data(role)};

    if (base_data != QVariant{})
        return base_data;

    if (role == SexRole)
        return sex;

    return QVariant{};
}

void c_infant::setData(const QVariant &value, int role)
{
    c_person::setData(value, role);

    if (role == SexRole)
        sex = c_senior_citizen::sexes(value.toInt());
}

void c_infant::read(const QJsonObject& json)
{
    c_person::read(json);

    if (json.contains("sex") && json["sex"].isDouble())
        sex = c_senior_citizen::sexes(json["sex"].toInt());
}

void c_infant::write(QJsonObject& json) const
{
    c_person::write(json);

    json["sex"] = sex;
}

bool c_infant::is_completed() const
{
    if (!c_person::is_completed())
        return false;

    return true;
}

}
}
