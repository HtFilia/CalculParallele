#include <iostream>
#include "PricingResults.hpp"
#include "PnlVectToJson.hpp"

std::ostream& operator<<(std::ostream &stm, const PricingResults &res)
{
    stm << '{';
    stm << "\"iterations\": " << res.nb_tirage;
    stm << ", \"price\": " << res.price;
    stm << ", \"priceStdDev\": " << res.priceStdDev;
    stm << ", \"time\": " << ((float)res.time)/CLOCKS_PER_SEC;
    stm << '}';
    return stm;
}