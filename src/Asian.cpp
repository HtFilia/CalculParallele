/**
* \file Asian.cpp
*
* \brief Fichier source de la classe Asian héritant de la classe Option.
*
* \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
* Fait le 11.09.2019
*
*/

#include <jlparser/parser.hpp>

#include "Option.hpp"
#include "Asian.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"

Asian::Asian(double strike, double maturity, int size, int nbTimeSteps, PnlVect* lambda) {
    T_ = maturity;
    size_ = size;
    nbTimeSteps_ = nbTimeSteps;
    K_ = strike;
    lambda_ = lambda;
}

Asian::Asian(const char *InputFile) {
    Parser *P = new Parser(InputFile);
    int size;
    P->extract("maturity", this->T_);
    P->extract("option size", size);
    this->size_ = size;
    P->extract("strike", this->K_);
    P->extract("timestep number", this->nbTimeSteps_);
    P->extract("payoff coefficients", this->lambda_, size);
}

Asian::~Asian() {
    if (lambda_) {
        pnl_vect_free(&lambda_);
    }
};

double Asian::payoff(const PnlMat *path) {
    double value = 0;
    for (size_t i = 0; i <= nbTimeSteps_; i++) {
        for (int j = 0; j < size_; j++) {
            value += GET(lambda_, j) * MGET(path, i, j);
        }
    }
    double res = MAX(value / (nbTimeSteps_ + 1) - K_, 0);
    return res;
}
