#include <wobjectimpl.h>

#include "rent_item.hpp"

namespace Data
{
W_OBJECT_IMPL(rent_item)

rent_item::rent_item(QObject *parent)
    : base_item{parent}
{
}

void rent_item::clear()
{
    marketPrice = 500000;
    emit marketPriceChanged();
    monthly = 1000;
    emit monthlyChanged();
    bou = 0;
    emit bouChanged();
    rva = 0;
    emit rvaChanged();
    dab = 0;
    emit dabChanged();
    estimation = 0;
    emit estimationChanged();
//    int dam{0};
//    emit damChanged();
}

int rent_item::floor_to(double number, int multiple) const
{
    return std::floor((number + multiple / 2) / multiple) * multiple;
}

void rent_item::from_expectency(double expectency)
{
    using namespace std;

    int pva = floor_to(marketPrice / pow(1 + INTEREST, expectency), 1000);
    setDab(marketPrice - pva);

    double ae{0};
    if (V == 1)
        ae = expectency;
    else
    {
        double ve{1 - pow(V, expectency)};
        ae = ve / (1 - V) - 11 / 24 * ve;
    }

    setRva(12 * monthly * ae);
    setBou(marketPrice - dab - rva);
    setEstimation(dab + bou);
}

int rent_item::getmarketPrice() const
{
    return marketPrice;
}

void rent_item::setmarketPrice(int newmarketPrice)
{
    if (marketPrice == newmarketPrice)
        return;
    marketPrice = newmarketPrice;
    emit marketPriceChanged();
}

int rent_item::getMonthly() const
{
    return monthly;
}

void rent_item::setMonthly(int newMonthly)
{
    if (monthly == newMonthly)
        return;
    monthly = newMonthly;
    emit monthlyChanged();
}

int rent_item::getRva() const
{
    return rva;
}

void rent_item::setRva(int newRva)
{
    if (rva == newRva)
        return;
    rva = newRva;
    emit rvaChanged();
}

int rent_item::getBou() const
{
    return bou;
}

void rent_item::setBou(int newBou)
{
    if (bou == newBou)
        return;
    bou = newBou;
    emit bouChanged();}

int rent_item::getDab() const
{
    return dab;
}

void rent_item::setDab(int newDab)
{
    if (dab == newDab)
        return;
    dab = newDab;
    emit dabChanged();
}

int rent_item::getEstimation() const
{
    return estimation;
}

void rent_item::setEstimation(int newEstimation)
{
    if (estimation == newEstimation)
        return;
    estimation = newEstimation;
    emit estimationChanged();
}

//int rent_item::getDam() const
//{
//    return dam;
//}

//void rent_item::setDam(int newDam)
//{
//    if (dam == newDam)
//        return;
//    dam = newDam;
//    emit damChanged();
//}

}