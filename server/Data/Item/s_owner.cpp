#include "s_owner.hpp"

namespace Data
{
namespace People
{
s_owner::s_owner()
    : owner_item{}
    , s_infant{}
    , ads{address}
{
}

void s_owner::set(const nanodbc::result& res)
{
    s_infant::set(res);

    try
    {
        if (!res.is_null("BirthDay"))
            birthDay = server::utils::from_db_date(res.get<std::string>("BirthDay"));
    }
    catch (...) {}

    try
    {
        if (!res.is_null("CivilStatus"))
            civilStatus = civilStates(res.get<int>("CivilStatus"));
    }
    catch (...) {}

    try
    {
        if (!res.is_null("AVS"))
            avs = res.get<std::string>("AVS");
    }
    catch (...) {}

    ads.set(res);
}

const std::string s_owner::insert(const s_user& usr, const s_account* acnt) const
{
    std::string str{"INSERT INTO BaseOwner "
                    "(OwnerType, Sex, OwnerAccountId) "
                    "OUTPUT inserted.id "
                    "VALUES('Owner', "
                    + std::to_string(sex) +
                ", "
                + std::to_string(acnt->id) +
                "); "};

    acnt->foreign_update(str, acnt);
    acnt->condition(str, usr, acnt);

    return str;
}

const std::string s_owner::select(const s_user& usr, const s_account* acnt) const
{
    return search(usr, acnt) + " AND  b.Id = " + std::to_string(id);
}

const std::string s_owner::update(const s_user& usr, const s_account* acnt) const
{
    return "UPDATE BaseOwner SET "
            + s_infant::fields() +
            ", BirthDay = '"
            + server::utils::to_db_date(birthDay) +
            "', CivilStatus = "
            + std::to_string(civilStatus) +
            ", AVS = '"
            + avs +
            "', "
            + ads.fields() +
            " WHERE Id = "
            + std::to_string(id) +
            "; ";
}

const std::string s_owner::remove(const s_user &usr, const s_account *acnt) const
{
    return "DELETE FROM BaseOwner WHERE Id = "
            + std::to_string(id);
}

void s_owner::foreign_update(std::string& query, bool complete, const s_account* acnt)
{
    std::string str{server::utils::update_flag(
                    account_item::OwnersCompleted,
                    "State",
                    complete)};
    acnt->foreign_update(str, complete, acnt);
    query.append(str);
}

void s_owner::condition(std::string& query, const s_user& usr, const s_account* acnt)
{
    acnt->condition(query, usr, acnt);
}

void s_owner::update_reply(nanodbc::result& res, Value& json, const s_account* acnt)
{
    acnt->update_reply(res, json);

    try
    {
        if (!res.is_null("Acronym"))
            json["acronym"] = res.get<std::string>("Acronym");
    }
    catch (...) {}
}

}
}
