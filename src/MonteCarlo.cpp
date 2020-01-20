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

/**
* Calcule le prix de l'option à la date 0
*
* @param[out] prix valeur de l'estimateur Monte Carlo
* @param[out] ic largeur de l'intervalle de confiance
*/
void MonteCarlo::price(double &prix, double &ic) {
    //TODO: PARALLELISER MONTE CARLO
    PnlMat* path = pnl_mat_create(opt_->getTimeSteps() + 1, mod_->getSize());
    prix = 0;
    double var = 0;

    for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, opt_->getMaturity(), opt_->getTimeSteps(), rng_);
        double tmp = opt_->payoff(path);
        prix += tmp;
        var += SQR(tmp);
    }

    prix = exp(- mod_->getR() * opt_->getMaturity()) * prix / nbSamples_;
    var = exp(-2 * mod_->getR() * opt_->getMaturity()) * var / nbSamples_ - SQR(prix);
    ic = sqrt(var / (double)nbSamples_);

    //free
    pnl_mat_free(&path);
}