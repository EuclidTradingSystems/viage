#include <QVariant>
#include <QDate>

#include "senior_citizen.hpp"
#include <client_utils.hpp>

using namespace client_utils;

namespace Data
{
namespace People
{
senior_citizen::senior_citizen()
    : birthDay{QDate::currentDate().addYears(-AGE_MIN)}
{}

QHash<int, QByteArray> senior_citizen::roleNames()
{
    QHash<int, QByteArray> names;

    names[BirthDayRole] = "birthDay";
    names[SexRole] = "sex";

    return names;
}

QVariant senior_citizen::data(int role) const
{
    if (role == BirthDayRole)
        return birthDay;
    else if (role == SexRole)
        return sex;

    return QVariant{};
}

void senior_citizen::setData(const QVariant &value, int role)
{
    if (role == BirthDayRole)
        birthDay = value.toDate();
    else if (role == SexRole)
        sex = sexes(value.toInt());
}

}

}
