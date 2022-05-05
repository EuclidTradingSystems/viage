#include <QVector>
#include <QFile>

#include "life_expectency.hpp"
#include <Lists/simple_item_list.hpp>
#include <Items/senior_citizen_item.hpp>

namespace Calculator
{

life_expectency::life_expectency(
        Data::simple_item_list<Data::People::senior_citizen_item>*
        senior_citizens)
    : seniors{senior_citizens}
{
    readMortality(":/ofs.csv");
}

void life_expectency::readMortality(const QString& path)
{
    QFile csv{path};

    if (!csv.open(QIODevice::ReadOnly))
    {
        qDebug() << "readMortality error :" << csv.errorString();
        return;
    }

    while (!csv.atEnd())
    {
        auto line{csv.readLine()};
        auto splitedLine{line.split(',')};

        int key{splitedLine[2].toInt()};
        std::array values = {splitedLine[3].toDouble(), splitedLine[4].toDouble()};

        mortality.insert(std::pair{key, values});
    }
}

double life_expectency::get_expectency()
{
    int count = seniors->items().size();

    data x_data{seniors->item_at(0)};

    data y_data{};
    if (count == 2)
        y_data = data{seniors->item_at(1)};

    double lx{1};
    double ex{lx};

    double ly{lx};
    double ey{lx};
    double lx_plus_y{lx};
    double ex_plus_y{lx};

    for (int i = 1; i <= 125; i++)
    {
        lx = lx * (1 - get_qx(x_data, i));
        ex += lx;

        if (count == 2)
        {
            ly = ly * (1 - get_qx(y_data, i));
            ey += ly;

            lx_plus_y = lx + ly - lx * ly;
            ex_plus_y += lx_plus_y;
        }
    }

    ex -= 0.5;
    ey -= 0.5;
    ex_plus_y -= 0.5;

    if (count == 1)
        return ex;

    return ex_plus_y;
}

double life_expectency::get_pers(const data& d, int index, int plus)
{
    int add = d.age_trunc + index + plus;
    int key = d.thousand_bDAy + std::min(add, AGE_MAX);
    return mortality[key][d.sex];
}

double life_expectency::get_qx(const data& d, int index)
{
    return d.age_diff * get_pers(d, index, 1) + (1 - d. age_diff) * get_pers(d, index);
}

}