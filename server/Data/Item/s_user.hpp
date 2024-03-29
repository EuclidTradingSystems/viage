#ifndef S_USER_HPP
#define S_USER_HPP

#include "s_person.hpp"
#include "s_address.hpp"
#include <Item/user_item.hpp>
#include <drogon/orm/Result.h>
#include <trantor/utils/Date.h>

namespace Data
{
namespace People
{
struct s_user final : public user_item
                    , public s_person<user_item>
{
    s_user();

    static const constexpr auto table{"User"};

    void set(const Row& row);
    void set(const Result& res) { s_base_data::set(res); };

    const std::string fields() const;
    const std::string insert(const s_user& usr) const;
    const std::string select(const s_user& usr) const;
    const std::string update(const s_user& usr) const;

    static void foreign_update(std::string& query, bool complete) {};
    static void select_updated(std::string& query) {};
    static void update_reply(const Result& res, Json::Value& json) {};

    static const constexpr std::string from_session_id(const std::string& session_id)
    {
        return "SELECT "
               "Id, "
               "Clearance "
               "FROM User "
               "WHERE SessionId = '"
               + session_id +
               "' AND IsLocked = FALSE";
    };

    static const constexpr std::string search(const s_user& usr)
    {
        if (usr.clearance < Administrator) return {};

        return "SELECT "
               "a.Id, "
               "FirstName, "
               "LastName, "
               "Login, "
               "EMail, "
               "Phone, "
               "Clearance, "
               "Beneficiary, "
               "Bic, "
               "Iban, "
               "Street, "
               "City, "
               "Canton, "
               "Zip, "
               "a.CompanyId, "
               "TeamId, "
               "IsLocked, "
               "b.Name, "
               "c.Caption "
               "FROM ((User a "
               "LEFT JOIN Company b "
               "ON a.CompanyId = b.Id) "
               "LEFT JOIN Team c "
               "ON a.TeamId = c.Id) ";
    };

protected:
    Places::s_address sa;
};

}
}

#endif // S_USER_HPP
