#include <QVariant>
#include <QDate>

#include "c_senior_citizen.hpp"
#include <client_utils.hpp>

using namespace client_utils;

namespace Data
{
namespace People
{
c_senior_citizen::c_senior_citizen()
    : senior_citizen_item{}
{
    const auto minAge{QDate::currentDate().addYears(-AGE_MIN)};
    birthDay = to_date(minAge);
}

QHash<int, QByteArray> c_senior_citizen::roleNames()
{
    QHash<int, QByteArray> names;

    names[BirthDayRole] = "birthDay";
    names[SexRole] = "sex";

    return names;
}

QVariant c_senior_citizen::data(int role) const
{
    if (role == BirthDayRole)
        return to_QDate(birthDay);
    else if (role == SexRole)
        return QVariant(sex);

    return QVariant();
}

void c_senior_citizen::setData(const QVariant &value, int role)
{
    if (role == BirthDayRole)
        birthDay = to_date(value.toDate());
    else if (role == SexRole)
        sex = sexes(value.toInt());
}

}

}
