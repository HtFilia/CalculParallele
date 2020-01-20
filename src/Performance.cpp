/**
* \file Performance.cpp
*
* \brief Fichier source de la classe Performance
*
* \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
* Fait le 11.09.2019
*
*/

#include <iostream>

#include "Option.hpp"
#include "Performance.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"
#include "jlparser/parser.hpp"

Performance::Performance(double maturity, int size, int nbTimeSteps, PnlVect* lambda)
{
    T_ = maturity;
    size_ = size;
    nbTimeSteps_ = nbTimeSteps;
    lambda_ = lambda;
}

Performance::Performance(const char *InputFile)
{
    Parser *P = new Parser(InputFile);
    int size;
    P->extract("maturity", this->T_);
    P->extract("option size", size);
    this->size_ = size;
    P->extract("timestep number", this->nbTimeSteps_);
    P->extract("payoff coefficients", this->lambda_, size);
}

Performance::~Performance(){
    if (lambda_) {
        pnl_vect_free(&lambda_);
    }
}

double Performance::payoff(const PnlMat *path)
{
    double res = 1;
    double oldValue;
    double value = 0;
    for (int j = 0; j < size_; j++) {
        value += GET(lambda_, j) * MGET(path, 0, j);
    }
    for (size_t i = 1; i <= nbTimeSteps_; i++) {
        oldValue = value;
        value = 0;
        for (int j = 0; j < size_; j++) {
            value += GET(lambda_, j) * MGET(path, i, j);
        }
        res += MAX(value / oldValue - 1, 0);
    }
    return res;
}
