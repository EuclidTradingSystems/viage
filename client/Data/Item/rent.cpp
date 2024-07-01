#include "qdatetime.h"

#include <wobjectimpl.h>

#include "rent.hpp"

namespace Data
{

#define INTEREST 0.0425
#define I -0.0225
#define V 1/(1 + I)

rent::rent(QObject* parent)
    : base_data{parent}
    , marketPrice{1500000}
    , birthDay{QDate::currentDate()}
{}

void rent::clear()
{
    setBirthDay(QDate::currentDate());
    setmarketPrice(1500000);
    setMonthly(0);
    setBou(0);
    setRva(0);
    setDab(0);
    setPva(0);
//    setDam{0};
}

int rent::floor_to(double number, int multiple) const
{
    return std::floor((number + multiple / 2) / multiple) * multiple;
}

void rent::from_expectency(double expectency)
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
        ae = ve / (1 - V) - 11.f / 24.f * ve;
    }

    setRva(12 * monthly * ae);
    setBou(marketPrice - dab - rva);
    setPva(dab + bou);
}

int rent::getmarketPrice() const
{
    return marketPrice;
}

void rent::setmarketPrice(int newmarketPrice)
{
    if (marketPrice == newmarketPrice)
        return;
    marketPrice = newmarketPrice;
    emit marketPriceChanged();
}

int rent::getMonthly() const
{
    return monthly;
}

void rent::setMonthly(int newMonthly)
{
    if (monthly == newMonthly)
        return;
    monthly = newMonthly;
    emit monthlyChanged();
}

int rent::getRva() const
{
    return rva;
}

void rent::setRva(int newRva)
{
    if (rva == newRva)
        return;
    rva = newRva;
    emit rvaChanged();
}

int rent::getBou() const
{
    return bou;
}

void rent::setBou(int newBou)
{
    if (bou == newBou)
        return;
    bou = newBou;
    emit bouChanged();}

int rent::getDab() const
{
    return dab;
}

void rent::setDab(int newDab)
{
    if (dab == newDab)
        return;
    dab = newDab;
    emit dabChanged();
}

int rent::getPva() const
{
    return pva;
}

void rent::setPva(int newPva)
{
    if (pva == newPva)
        return;
    pva = newPva;
    emit pvaChanged();
}

const QDate rent::getBirthDay() const
{
    return birthDay;
}

void rent::setBirthDay(const QDate& newBirthDay)
{
    if (birthDay == newBirthDay)
        return;
    birthDay = newBirthDay;
    emit birthDayChanged();
}

//int rent::getDam() const
//{
//    return dam;
//}

//void rent::setDam(int newDam)
//{
//    if (dam == newDam)
//        return;
//    dam = newDam;
//    emit damChanged();
//}

W_OBJECT_IMPL(rent)

}
