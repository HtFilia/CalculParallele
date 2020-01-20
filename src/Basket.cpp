/**
* \file Basket.cpp
*
* \brief Fichier source de la classe Basket
*
* \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
* Fait le 11.09.2019
*
*/

#include <fstream>
#include <iostream>

#include "Basket.hpp"
#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"
#include "jlparser/parser.hpp"

using namespace std;

Basket::Basket(double strike, double maturity, int size, int nbTimeSteps, PnlVect* lambda)
{
    T_ = maturity;
    size_ = size;
    nbTimeSteps_ = nbTimeSteps;
    K_ = strike;
    lambda_ = lambda;
}

Basket::Basket(const char *InputFile)
{
    Parser *P = new Parser(InputFile);
    int size;
    P->extract("maturity", this->T_);
    P->extract("option size", size);
    this->size_ = size;
    P->extract("strike", this->K_);
    P->extract("timestep number", this->nbTimeSteps_);
    P->extract("payoff coefficients", this->lambda_, size);
    delete P;
}

Basket::~Basket() {}

double Basket::payoff(const PnlMat *path)
{
    double value = 0;
    for (size_t i = 0; i < size_; i++) {
        value += GET(lambda_, i) * MGET(path, nbTimeSteps_, i);
    }
    double res = MAX(value - K_, 0);
    return res;
}
