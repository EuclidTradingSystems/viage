#ifndef EXPECTENCY
#define EXPECTENCY

#include <QDate>
#include <Item/c_senior_citizen.hpp>
#include <Item/c_base_item.hpp>
#include <List/c_simple_list.hpp>

namespace Data
{
template <typename T>
class simple_item_list;
}

namespace Calculator
{
using namespace Data;
using namespace People;

class life_expectency
{
public:
    explicit life_expectency(c_simple_list<c_senior_citizen> *senior_citizens);

    double get_expectency(const QDate& date);

private:
    c_simple_list<c_senior_citizen>* seniors;

    struct dto : private c_base_item
    {
        dto() {};
        dto(const senior_citizen_item& senior, const QDate& date)
            : sex{senior.sex}
            , thousand_bDAy{to_QDate(senior.birthDay).year() * 1000}
            , age{to_QDate(senior.birthDay).daysTo(date) / 365.25}
            , age_trunc{std::trunc(age)}
            , age_diff{age - age_trunc}
        {}
        senior_citizen_item::sexes sex{senior_citizen_item::M};
        int thousand_bDAy{0};
        double age{0};
        double age_trunc{0};
        double age_diff{0};
    };

    std::map<int, std::array<double, 2>> mortality{};
    int mortality_offset{0};

    void readMortality(const QString& path);
    double get_mortality();

    double get_pers(const dto& d, int index, int plus = 0);
    double get_qx(const dto& d, int index);
};

}

#endif // EXPECTENCY