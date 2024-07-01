#include <QVariant>

#include "user.hpp"

namespace Data
{
namespace People
{
user::user()
    : clearance{Vendor}
{}

QHash<int, QByteArray> user::roleNames()
{
    QHash<int, QByteArray> names{person::roleNames()};

    names[CompanyRole] = "company";
    names[ClearanceRole] = "clearance";
    names[TeamNameRole] = "team";
    names[BeneficiaryRole] = "beneficiary";

    names.insert(Places::address::roleNames());

    names[IbanRole] = "iban";
    names[BicRole] = "bic";
    names[CompanyIdRole] = "companyId";
    names[TeamIdRole] = "teamId";
    names[LockedRole] = "isLocked";
    names[CompletedRole] = "completed";

    return names;
}

QVariant user::data(int role) const
{
    QVariant extra_data{person::data(role)};

    if (extra_data != QVariant{})
        return extra_data;

    switch (role)
    {
    case CompanyRole:
        return company;
    case ClearanceRole:
        return clearance;
    case TeamNameRole:
        return team;
    case BeneficiaryRole:
        return beneficiary;
    case IbanRole:
        return iban;
    case BicRole:
        return bic;
    case CompanyIdRole:
        return company_id;
    case TeamIdRole:
        return team_id;
    case LockedRole:
        return isLocked;
    case CompletedRole:
        return is_completed();
    }

    extra_data = ca.data(role);

    if (extra_data != QVariant{})
        return extra_data;

    return QVariant{};
}

void user::setData(const QVariant &value, int role)
{
    person::setData(value, role);

    switch (role)
    {
    case CompanyRole:
        company = value.toString();
        break;
    case ClearanceRole:
        clearance = clearances(value.toInt());
        break;
    case TeamNameRole:
        team = value.toString();
        break;
    case BeneficiaryRole:
        beneficiary = value.toString();
        break;
    case IbanRole:
        iban = value.toString();
        break;
    case BicRole:
        bic = value.toString();
        break;
    case CompanyIdRole:
        company_id = value.toInt();
        break;
    case TeamIdRole:
        team_id = value.toInt();
        break;
    case LockedRole:
        isLocked = value.toBool();
        break;
    }

    ca.setData(value, role);
}

void user::read(const QJsonObject &json)
{
    person::read(json);

    if (json.contains("company") && json["company"].isString())
        company = json["company"].toString();

    if (json.contains("clearance") && json["clearance"].isDouble())
        clearance = clearances(json["clearance"].toInt());

    if (json.contains("team") && json["team"].isString())
        team = json["team"].toString();

    if (json.contains("beneficiary") && json["beneficiary"].isString())
        beneficiary = json["beneficiary"].toString();

    if (json.contains("address") && json["address"].isObject())
        ca.read(json["address"].toObject());

    if (json.contains("iban") && json["iban"].isString())
        iban = json["iban"].toString();

    if (json.contains("bic") && json["bic"].isString())
        bic = json["bic"].toString();

    if (json.contains("companyId") && json["companyId"].isDouble())
        company_id = json["companyId"].toInt();

    if (json.contains("teamId") && json["teamId"].isDouble())
        team_id = json["teamId"].toInt();

    if (json.contains("isLocked") && json["isLocked"].isBool())
        isLocked = json["isLocked"].toBool();
}

void user::write(QJsonObject& json) const
{
    person::write(json);

    json["company"] = company;
    json["clearance"] = clearance;
    json["team"] = team;
    json["beneficiary"] = beneficiary;

    QJsonObject jsonAddress{};
    ca.write(jsonAddress);

    json["address"] = jsonAddress;
    json["iban"] = iban;
    json["bic"] = bic;
    json["companyId"] = company_id;
    json["teamId"] = team_id;
    json["isLocked"] = isLocked;
}

bool user::is_completed() const
{
    if(!person::is_completed())
        return  false;

    if(company == "")
        return false;

    if(team == "")
        return false;

    if(clearance == None)
        return false;

    if(beneficiary == "")
        return false;

    if(!ca.is_completed())
        return false;

    if(iban == "")
        return false;

    if(bic == "")
        return false;

    return true;
}

}
}
