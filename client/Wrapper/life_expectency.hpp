#ifndef EXPECTENCY
#define EXPECTENCY

#include <QDate>
#include <Item/senior_citizen.hpp>
#include <client_utils.hpp>

using namespace client_utils;

namespace Data
{
template <typename T>
class list;
}

namespace Calculator
{
using namespace Data;
using namespace People;

class life_expectency
{
public:
    explicit life_expectency(list<senior_citizen>* senior_citizens);

    double get_expectency(const QDate& date);

private:
    list<senior_citizen>* seniors;

    struct dto
    {
        dto() {}
        dto(const senior_citizen& senior, const QDate& date)
            : sex{senior.sex}
            , thousand_bDAy{senior.birthDay.year() * 1000}
            , age{senior.birthDay.daysTo(date) / 365.25}
            , age_trunc{std::trunc(age)}
            , age_diff{age - age_trunc}
        {}
        senior_citizen::sexes sex{senior_citizen::M};
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
