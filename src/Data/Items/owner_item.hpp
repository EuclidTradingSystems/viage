#ifndef OWNER_ITEM_HPP
#define OWNER_ITEM_HPP

#include "infant_item.hpp"
#include "address_item.hpp"

namespace Data
{
struct url_list;

namespace People
{

struct owner_item : public infant_item
                  , public Places::address_item
{
    owner_item();

    const constexpr char* key() { return "owner"; };
    static const constexpr auto qmlName{"Owner"};

    QDate birthDay;

    enum civilStates
    {
        None = 0,
        Single,
        Maried,
        Registered,
        Separated,
        Divorced,
        Disolved,
        Widow
    };

    civilStates civilStatus{None};
    QString avs{""};

    enum roles
    {
        // following infant_items roles
        BirthDayRole = Qt::UserRole + 6,
        CivilStatusRole,
        AVSRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    bool is_completed() const;
};

}
}

#endif // OWNER_ITEM_HPP
