#include <QVariant>

#include "contact.hpp"

namespace Data
{
namespace People
{
QHash<int, QByteArray> contact::roleNames()
{
    QHash<int, QByteArray> names = infant::roleNames();

    names[IsInfantRole] = "isInfant";

    return names;
}

QVariant contact::data(int role) const
{
    QVariant base_data{infant::data(role)};

    if (base_data != QVariant{})
        return base_data;

    if (role == IsInfantRole)
        return isInfant;

    return QVariant{};
}

void contact::setData(const QVariant &value, int role)
{
    infant::setData(value, role);

    if (role == IsInfantRole)
        isInfant = value.toBool();
}

void contact::read(const QJsonObject& json)
{
    infant::read(json);

    if (json.contains("isInfant") && json["isInfant"].isBool())
        isInfant = json["isInfant"].toBool();
}

void contact::write(QJsonObject& json) const
{
    infant::write(json);

    json["isInfant"] = isInfant;
}

bool contact::is_completed() const
{
    if (!infant::is_completed())
        return false;

    return true;
}

}
}
