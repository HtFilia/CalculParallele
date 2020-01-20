/**
 * \file MonteCarlo.cpp
 *
 *
 * \brief Fichier source de la classe MonteCarlo
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#include <math.h>
#include <iostream>

#include "MonteCarlo.hpp"

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples) {
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}

MonteCarlo::~MonteCarlo(){}

/**
* Calcule le prix de l'option à la date 0
*
* @param[out] prix valeur de l'estimateur Monte Carlo
* @param[out] ic largeur de l'intervalle de confiance
*/
void MonteCarlo::price(double &prix, double &ic) {
    //TODO: PARALLELISER MONTE CARLO
    PnlMat* path = pnl_mat_create(opt_->nbTimeSteps_ + 1, mod_->size_);
    prix = 0;
    double var = 0;

    for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        double tmp = opt_->payoff(path);
        prix += tmp;
        var += SQR(tmp);
    }

    prix = exp(- mod_->r_ * opt_->T_) * prix / nbSamples_;
    var = exp(-2 * mod_->r_ * opt_->T_) * var / nbSamples_ - SQR(prix);
    ic = sqrt(var / (double)nbSamples_);

    //free
    pnl_mat_free(&path);
}