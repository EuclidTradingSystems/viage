#ifndef S_EXTERIOR_HPP
#define S_EXTERIOR_HPP

#include "s_account.hpp"
#include <server_utils.hpp>
#include <Item/exterior_item.hpp>

namespace Data
{
namespace Places
{
struct s_exterior final : public exterior_item
                        , public s_base_data
{
    s_exterior();

    void set(const Row& row) override;
    void set(const Result& res) { s_base_data::set(res); };

    const std::string insert(const People::s_user& usr, const s_account* acnt = nullptr) const;
    const std::string select(const People::s_user& usr, const s_account* acnt = nullptr) const;
    const std::string update(const People::s_user& usr, const s_account* acnt = nullptr) const;

    static void foreign_update(std::string& query,
                               bool complete,
                               const s_account* acnt = nullptr);

    static void condition(std::string& query,
                          const People::s_user& usr,
                          const s_account* acnt);

    static void select_updated(std::string& query,
                               const s_account* acnt = nullptr);

    static void update_reply(const Result& res,
                             Json::Value& json,
                             const s_account* acnt);

    static const constexpr std::string search(const People::s_user& usr,
                                              const s_account* acnt)
    {
        return "SELECT DISTINCT "
               "a.HasParking, "
               "ParkingSpots, "
               "ParkingType, "
               "ParkingSurface, "
               "TerrainDescription, "
               "TerrainSurface, "
               "Rating, "
               "Equipements "
               "FROM Account a, "
               "User u "
               "WHERE a.Id = "
                + std::to_string(acnt->id) +
                " "
                + server_utils::clearance_close(usr);
    }
};

}
}

#endif // S_EXTERIOR_HPP
