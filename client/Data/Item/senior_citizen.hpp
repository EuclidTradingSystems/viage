#ifndef SENIOR_CITIZEN_HPP
#define SENIOR_CITIZEN_HPP

#include "qdatetime.h"

namespace Data
{
namespace People
{
#define AGE_MIN 70
#define AGE_MAX 120

struct senior_citizen final
{
    senior_citizen();

    static consteval auto key() { return "seniorCitizen"; }
    static consteval auto qmlName() { return "SeniorCitizen"; }
    static consteval auto uri() { return "People"; }

    enum sexes
    {
        M = 0,
        F
    };

    enum roles
    {
        BirthDayRole = Qt::UserRole,
        SexRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    bool is_completed() const;

    QDate birthDay;
    sexes sex;
};

}
}

#endif // SENIOR_CITIZEN_ITEM_HPP
