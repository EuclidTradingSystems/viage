#include <QJsonArray>
#include <QVariant>

#include "company.hpp"
#include <list.hpp>
#include "team.hpp"

namespace Data
{
QHash<int, QByteArray> company::roleNames()
{
    QHash<int, QByteArray> names;

    names[IdRole] = "id";
    names[NameRole] = "name";
    names[TeamsRole] = "teams";

    return names;
}

QVariant company::data(int role) const
{
    switch (role)
    {
    case IdRole:
        return id;
    case NameRole:
        return name;
    case TeamsRole:
        return teams;
    }

    return QVariant{};
}

void company::setData(const QVariant& value, int role)
{
    if (role == NameRole)
        name = value.toString();
}


bool company::update(list<team>* ti)
{
    QJsonArray arr{};
    ti->write(arr);
    if (teams == arr)
        return false;

    teams = arr;
    return true;
}

QJsonArray company::get(list<team>* ti) const
{
    return teams;
}

bool company::is_completed() const
{
    if (name.isEmpty())
        return false;

    return true;
}

}
