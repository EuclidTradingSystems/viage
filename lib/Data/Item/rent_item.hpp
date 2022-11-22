#ifndef RENT_ITEM_H
#define RENT_ITEM_H

#include <json/json.h>
#include <base_data.hpp>

namespace Data
{
class rent_item : virtual public base_data
{
public:
    explicit rent_item();

    const char* key() const noexcept override { return "rent"; };

    void from_expectency(double expectency);
    int floor_to(double number, int multiple) const;
    void calculate();
    void writeToFile();

    std::string birthDay{""};
    int marketPrice{1500000};
    int monthly{0};

    int rva{0};
    int bou{0};
    int dab{0};
    int pva{0};
    //    int dam{0};
};

}

#endif // RENT_ITEM_H
