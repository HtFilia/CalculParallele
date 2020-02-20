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

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples)
{
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    path = pnl_mat_create(opt->nbTimeSteps_+1, mod->size_);
}

MonteCarlo::~MonteCarlo()
{
    pnl_mat_free(&path);
}


void MonteCarlo::price(int &nb_tirage, double &prix, double &ic) {
    //TODO: PARALLELISER MONTE CARLO
    prix = 0;
    double var = 0;

    for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        double tmp = opt_->payoff(path);
        prix += tmp;
        var += tmp * tmp;
    }

    nb_tirage = nbSamples_;
    prix = exp(- mod_->r_ * opt_->T_) * prix / nbSamples_;
    var = exp(-2 * mod_->r_ * opt_->T_) * var / nbSamples_ - SQR(prix);
    ic = sqrt(var / (double)nbSamples_);
}

void MonteCarlo::price_with_prec(double prec, int &nb_tirage, double &prix, double &ic) {
    //TODO: PARALLELISER MONTE CARLO
    prix = 0;
    double var = 0;
    bool test = true;

    for (int i = 1; test || i < 3; i++) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        double tmp = opt_->payoff(path);
        prix += tmp;
        var += tmp * tmp;
        test = (exp(- mod_->r_ * opt_->T_) * sqrt(var - SQR(prix) / i) / (double)i > prec);
        nb_tirage = i;
    }

    prix = exp(- mod_->r_ * opt_->T_) * prix / nb_tirage;
    var = exp(-2 * mod_->r_ * opt_->T_) * var / nb_tirage - SQR(prix);
    ic = sqrt(var / (double)nb_tirage);
}