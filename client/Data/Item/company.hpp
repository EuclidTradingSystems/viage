#ifndef COMPANY_HPP
#define COMPANY_HPP

#include "qnamespace.h"
#include "qjsonarray.h"

#include "base_item.hpp"

namespace Data
{
template <typename T>
struct list;

struct team;

struct company final : public base_item<company>
{
    company() = default;

    static consteval auto key() { return "company"; };
    static consteval auto qmlName() { return "Company"; };
    static consteval auto uri() { return "Data"; };

    enum roles
    {
        IdRole = Qt::UserRole,
        NameRole,
        TeamsRole
    };

    static QHash<int , QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    bool update(list<team>* ti);
    QJsonArray get(list<team>* ti) const;

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    bool is_completed() const override;

private:
    int id;
    QString name;
    QJsonArray teams;
};

}

#endif // COMPANY_ITEM_HPP
