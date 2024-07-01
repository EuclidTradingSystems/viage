#ifndef TEAM_HPP
#define TEAM_HPP

#include "qnamespace.h"

namespace Data
{
struct team final
{
    static const constexpr auto qmlName{"Team"};
    static const constexpr auto uri{"Data"};

    enum roles
    {
        IdRole = Qt::UserRole,
        CaptionRole,
        HasUsersRole
    };

    static QHash<int , QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

private:
    int id;
    QString caption;
    bool has_users;
};

}

#endif // TEAM_HPP
