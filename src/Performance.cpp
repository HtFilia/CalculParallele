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

/**
* \brief Constructeur du Performance Option.
*
*/
Performance::Performance(double maturity, int size, int nbTimeSteps, PnlVect* lambda) {
    this->T_ = maturity;
    this->size_ = size;
    this->nbTimeSteps_ = nbTimeSteps;
    this->lambda_ = lambda;
}

/**
* \brief Constructeur du Performance Option à partir d'un Fichier,
*        lecture à l'aide d'un Parser.
*
* @param[in] Le fichier contenant les paramètres de l'option Performance.
*
*/
Performance::Performance(const char *InputFile) {
    Parser *P = new Parser(InputFile);
    int size;
    P->extract("maturity", this->T_);
    P->extract("option size", size);
    this->size_ = size;
    P->extract("timestep number", this->nbTimeSteps_);
    P->extract("payoff coefficients", this->lambda_, size);
}

/**
* Destructeur
*/
//destructuer n'a pas à libérer le lambda, vous devez libérer les pnl_vect vous memes dans les tests selon moi
Performance::~Performance(){
    if (lambda_) {
        pnl_vect_free(&lambda_);
    }
}

void Performance::setLambda(PnlVect *lambda) {
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
double Performance::payoff(const PnlMat *path) {
    double res = 1;
    double oldValue;
    double value = 0;
    for (int j = 0; j < getSize(); j++) {
        value += GET(lambda_, j) * MGET(path, 0, j);
    }
    for (size_t i = 1; i <= getTimeSteps(); i++) {
        oldValue = value;
        value = 0;
        for (int j = 0; j < getSize(); j++) {
            value += GET(lambda_, j) * MGET(path, i, j);
        }
        res += MAX(value / oldValue - 1, 0);
    }
    return res;
    }
