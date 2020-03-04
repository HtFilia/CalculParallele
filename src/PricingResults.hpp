#pragma once
#include <string>
#include "pnl/pnl_vector.h"

class PricingResults
{
private:
    int nb_tirage;
    double price;
    double priceStdDev;
    clock_t time;

public:
    PricingResults(int nb_tirage, double price, double priceStdDev, clock_t time)
        : price(price)
        , priceStdDev(priceStdDev)
        , nb_tirage(nb_tirage)
        , time(time)
    { }

    ~PricingResults() { }

    PricingResults(const PricingResults &pr)
        : price(pr.price)
        , priceStdDev(pr.priceStdDev)
        , nb_tirage(pr.nb_tirage)
        , time(pr.time)
    { }

    int NbTirage() const
    {
        return nb_tirage;
    }

    double Price() const
    {
        return price;
    }

    double PriceStdDev() const
    {
        return priceStdDev;
    }

    clock_t Time() const
    {
        return time;
    }

    friend std::ostream& operator<<(std::ostream &stm, const PricingResults &res);
};
