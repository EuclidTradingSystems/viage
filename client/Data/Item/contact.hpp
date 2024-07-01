#ifndef CONTACT_HPP
#define CONTACT_HPP

#include "infant.hpp"
#include <Data/list.hpp>

namespace Data
{
namespace People
{
struct contact final : virtual public infant
{
    static consteval auto key() { return "contact"; }
    static consteval auto qmlName() { return "Contact"; }

    enum roles
    {
        // following infant_items roles
        IsInfantRole = Qt::UserRole + 5
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json);
    void write(QJsonObject &json) const;

    bool is_completed() const;

private:
    bool isInfant;
};
}

template<>
inline bool list<People::contact>::is_completed() const
{
    return is_empty_completed();
}
}

#endif // CONTACT_HPP
