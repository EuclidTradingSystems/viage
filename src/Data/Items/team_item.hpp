#ifndef TEAM_ITEM_H
#define TEAM_ITEM_H

#include <QJsonArray>

namespace Data
{

struct team_item
{
    team_item();

    const constexpr char* key() { return "team"; };
    static const constexpr auto qmlName{"Team"};
    static const constexpr auto uri{"Data"};

    int id{0};
    int number{0};
    bool has_users{false};

    enum roles
    {
        IdRole = Qt::UserRole,
        NumberRole,
        HasUsersRole
    };

    static QHash<int , QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    bool is_completed() const;
};

}

#endif // TEAM_ITEM_H