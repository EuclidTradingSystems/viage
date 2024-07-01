#ifndef USER_HPP
#define USER_HPP

#include "qnamespace.h"
#include "person.hpp"
#include "address.hpp"
#include "base_item.hpp"

namespace Data
{
namespace People
{
struct user final : virtual public base_item<user>
                  , virtual public person
{
    user();

    static consteval auto key() { return "user"; }
    static consteval auto qmlName() { return "User"; }

    enum clearances
    {
        None = 0,
        Vendor,
        Manager,
        Director,
        Administrator
    };

    enum roles
    {
        // following person roles
        CompanyRole = Qt::UserRole + 5,
        ClearanceRole,
        TeamNameRole,
        BeneficiaryRole,
        // gap left for address roles
        IbanRole = Qt::UserRole + 14,
        BicRole,
        CompanyIdRole,
        TeamIdRole,
        LockedRole,
        CompletedRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    bool is_completed() const override;

protected:
    Places::address ca;
    QString company;
    int company_id;
    clearances clearance;
    QString team;
    int team_id;
    QString beneficiary;
    QString iban;
    QString bic;
    bool isLocked;
};

}
}

#endif // USER_HPP
