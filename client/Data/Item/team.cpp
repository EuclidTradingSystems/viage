#include "qvariant.h"

#include "team.hpp"

namespace Data
{
QHash<int, QByteArray> Data::team::roleNames()
{
    QHash<int, QByteArray> names;

    names[IdRole] = "id";
    names[CaptionRole] = "caption";
    names[HasUsersRole] = "hasUsers";

    return names;
}

QVariant team::data(int role) const
{
    switch (role)
    {
    case IdRole:
        return id;
    case CaptionRole:
        return caption;
    case HasUsersRole:
        return has_users;
    }

    return QVariant{};
}

void team::setData(const QVariant& value, int role)
{
    if (role == CaptionRole)
        caption = value.toString();

    if (role == HasUsersRole)
        has_users = value.toBool();
}

}
