#ifndef RENT_HPP
#define RENT_HPP

#include <wobjectdefs.h>

#include "base_data.hpp"

namespace Data
{
class rent final : public base_data
{
W_OBJECT(rent)

public:
    explicit rent(QObject* parent = nullptr);

    static consteval auto key() { return "rentItem"; }
    static consteval auto qmlName() { return "RentItem"; }
    static consteval auto uri() { return "Data"; }

    void clear() override;
    W_SLOT(clear)

    void from_expectency(double expectency);

    void calculate()
    W_SIGNAL(calculate)

    void writeToFile()
    W_SIGNAL(writeToFile)

    void birthDayChanged()
    W_SIGNAL(birthDayChanged)
    void marketPriceChanged()
    W_SIGNAL(marketPriceChanged)
    void monthlyChanged()
    W_SIGNAL(monthlyChanged)
    void rvaChanged()
    W_SIGNAL(rvaChanged)
    void bouChanged()
    W_SIGNAL(bouChanged)
    void dabChanged()
    W_SIGNAL(dabChanged)
    void pvaChanged()
    W_SIGNAL(pvaChanged)
//    void damChanged()
//    W_SIGNAL(damChanged)

    const QDate getBirthDay() const;
    void setBirthDay(const QDate &newBirthDay);
    int getmarketPrice() const;
    void setmarketPrice(int newmarketPrice);
    int getMonthly() const;
    void setMonthly(int newMonthly);
    int getRva() const;
    void setRva(int newRva);
    int getBou() const;
    void setBou(int newBou);
    int getDab() const;
    void setDab(int newDab);
    int getPva() const;
    void setPva(int newPva);
//    int getDam() const;
//    void setDam(int newDam);

    W_PROPERTY(QDate, birthDay READ getBirthDay WRITE setBirthDay NOTIFY birthDayChanged)
    W_PROPERTY(int, marketPrice READ getmarketPrice WRITE setmarketPrice NOTIFY marketPriceChanged)
    W_PROPERTY(int, monthly READ getMonthly WRITE setMonthly NOTIFY monthlyChanged)
    W_PROPERTY(int, rva READ getRva NOTIFY rvaChanged)
    W_PROPERTY(int, bou READ getBou NOTIFY bouChanged)
    W_PROPERTY(int, dab READ getDab NOTIFY dabChanged)
    W_PROPERTY(int, pva READ getPva NOTIFY pvaChanged)
//    W_PROPERTY(int, dam READ getDam WRITE setDam NOTIFY damChanged)

private:
    QDate birthDay;
    int marketPrice;
    int monthly;

    int rva;
    int bou;
    int dab;
    int pva;
    //    int dam{0};

    int floor_to(double number, int multiple) const;
};

}

#endif // RENT_ITEM_H
