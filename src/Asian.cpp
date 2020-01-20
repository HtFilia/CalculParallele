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

/**
* \class Asian
*
* \brief Classe Asian représentant une Option de type Asiatique.
*
*/
/**
* \brief Constructeur du Basket Option.
*
*/
Asian::Asian(double strike, double maturity, int size, int nbTimeSteps, PnlVect* lambda) {
    this->T_ = maturity;
    this->size_ = size;
    this->nbTimeSteps_ = nbTimeSteps;
    this->K_ = strike;
    this->lambda_ = lambda;
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

/**
* \brief Destructeur de la classe Asian.
*
*/
Asian::~Asian() {
    if (lambda_) {
        pnl_vect_free(&lambda_);
    }
};


void Asian::setLambda(PnlVect *lambda) {
    lambda_ = lambda;
}

/**
* \brief Calcule le payoff de l'option Basket suivant le marché qu'on lui donne.
*
* @param[out] path le marché contenant les spots des sous-jacents
* 					aux différents temps étudiés.
*
* @return la valeur du payoff du Call.
*
*/
double Asian::payoff(const PnlMat *path) {
    double value = 0;
    for (size_t i = 0; i <= getTimeSteps(); i++) {
        for (int j = 0; j < getSize(); j++) {
            value += GET(lambda_, j) * MGET(path, i, j);
        }
    }
    double res = MAX(value / (getTimeSteps() + 1) - K_, 0);
    return res;
}
