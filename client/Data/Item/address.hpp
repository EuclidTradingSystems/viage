#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include "qjsonobject.h"

#include "base_item.hpp"

namespace Data
{
namespace Places
{
struct address_item;

struct address final : public base_item<address>
{
    address();

    static consteval auto key() { return "address"; };

    enum roles
    {
        // following after owner_item roles
        StreetRole = Qt::UserRole + 10,
        ZipRole,
        CantonRole,
        CityRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    bool is_completed() const override;

    QString street;
    QString city;
    int zip;
    QString canton;
};

}
}

#endif // ADDRESS_HPP
