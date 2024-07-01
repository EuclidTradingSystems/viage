#ifndef PERSON_HPP
#define PERSON_HPP

#include "qnamespace.h"

#include "base_item.hpp"

namespace Data
{
namespace People
{
struct person : base_item<person>
{
    static consteval auto key() { return "person"; }
    static consteval auto uri() { return "People"; }

    enum roles
    {
        IdRole = Qt::UserRole,
        FirstNameRole,
        LastNameRole,
        PhoneRole,
        EmailRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json) override;
    void write(QJsonObject &json) const override;

    int id;
    QString firstName;
    QString lastName;
    QString phone;
    QString eMail;

protected:
    person();
};

}
}

#endif // PERSON_HPP
