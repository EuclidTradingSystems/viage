#ifndef SENIOR_CITIZEN_ITEM_HPP
#define SENIOR_CITIZEN_ITEM_HPP

#include <json/json.h>
#include <base_data.hpp>

namespace Data
{
namespace People
{

#define AGE_MIN 70
#define AGE_MAX 120

struct senior_citizen_item : virtual public base_data<senior_citizen_item>
{
    static const constexpr auto key{"seniorCitizen"};

    enum sexes{ M, F };

    sexes sex{sexes(M)};
    std::string birthDay{""};

    bool is_completed() const override;

protected:
    explicit senior_citizen_item();
};

}
}

#endif // SENIOR_CITIZEN_ITEM_HPP
