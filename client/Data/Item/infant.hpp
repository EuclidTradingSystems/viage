#ifndef INFANT_HPP
#define INFANT_HPP

#include "qnamespace.h"

#include "base_item.hpp"
#include "person.hpp"
#include "senior_citizen.hpp"

namespace Data
{
namespace People
{
struct infant : virtual public base_item<infant>
              , virtual public person
{
    static consteval auto key() { return "infant"; }

    enum roles
    {
        // following infant_item roles
        SexRole = Qt::UserRole + 6
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    bool is_completed() const override;

    senior_citizen::sexes sex{senior_citizen::M};

protected:
    infant() = default;
};

}
}

#endif // INFANT_HPP
