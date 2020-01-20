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

/**
* \brief Constructeur du Basket Option.
*
*/
Basket::Basket(double strike, double maturity, int size, int nbTimeSteps, PnlVect* lambda) {
    this->T_ = maturity;
    this->size_ = size;
    this->nbTimeSteps_ = nbTimeSteps;
    this->K_ = strike;
    this->lambda_ = lambda;
}

Basket::Basket(const char *InputFile) {
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
//destructuer n'a pas à libérer le lambda, vous devez libérer les pnl_vect vous memes dans les tests selon moi
Basket::~Basket(){
    if (lambda_) {
        pnl_vect_free(&lambda_);
    }
}

/**
* \brief Getter du strike de l'Option, \refitem K_
*
* @return L'attribut \refitem K_
*
*/
double Basket::getK() const {
    return K_;
}

/**
* \brief Getter des coefficients de payoff de l'option, \refitem lambda_
*
* @return L'attribut \refitem lambda_
*
*/
PnlVect* Basket::getLambda() const {
    return lambda_;
}

void Basket::setLambda(PnlVect *lambda) {
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
double Basket::payoff(const PnlMat *path) {
    double value = 0;
    for (size_t i = 0; i < getSize(); i++) {
        value += GET(lambda_, i) * MGET(path, getTimeSteps(), i);
    }
    double res = MAX(value - K_, 0);
    return res;
}

/**
* \brief TODO
*
*/
void Basket::RedirectToFile(const char *path){
    ofstream file;
    file.open(path);
    file << "model type                   <string>     bs\n";
    file << "maturity                     <float>      " << getMaturity() << "\n";
    file << "option size                  <int>        " << getSize() << "\n";
    file << "strike                       <float>      " << K_ << "\n";
    file << "option type                  <string>     basket" << "\n";
    double payoffcoeff = 1/((double) getSize());
    file << "payoff coefficients          <vector>     " << payoffcoeff << "\n";
    file << "timestep number              <int>        " << nbTimeSteps_ << "\n";
    file.close();
}
