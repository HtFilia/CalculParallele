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

/**
 * Calcule le prix de l'option à la date t
 *
 * @param[in]  past contient la trajectoire du sous-jacent
 * jusqu'à l'instant t
 * @param[in] t date à laquelle le calcul est fait
 * @param[out] prix contient le prix
 * @param[out] ic contient la largeur de l'intervalle
 * de confiance sur le calcul du prix
 */
void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic) {
    PnlMat* path = pnl_mat_create(opt_->getTimeSteps() + 1, mod_->getSize());
    prix = 0;
    double var = 0;

    for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, t, opt_->getMaturity(), opt_->getTimeSteps(), rng_, past);
        double tmp = opt_->payoff(path);
        prix += tmp;
        var += SQR(tmp);
    }

    prix = exp(- mod_->getR() * (opt_->getMaturity() - t)) * prix / nbSamples_;
    var = exp(-2 *  mod_->getR() * (opt_->getMaturity() - t)) * var / nbSamples_ - SQR(prix);
    ic = sqrt(var / (double)nbSamples_);

    //free
    pnl_mat_free(&path);
}

/**
 * Calcule le delta de l'option à la date t
 *
 * @param[in] past contient la trajectoire du sous-jacent
 * jusqu'à l'instant t
 * @param[in] t date à laquelle le calcul est fait
 * @param[out] delta contient le vecteur de delta
 * @param[out] ic contient la largeur de l'intervalle
 * de confiance sur le calcul du delta
 */
void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic) {
    pnl_vect_set_zero(delta);
    pnl_vect_set_zero(ic);
    PnlVect *var = pnl_vect_create_from_scalar(mod_->getSize(), 0);

    double timestep = opt_->getMaturity() / (double) opt_->getTimeSteps();

    PnlMat *path = pnl_mat_create(opt_->getTimeSteps() + 1, mod_->getSize());
    PnlMat *shift_path = pnl_mat_create(opt_->getTimeSteps() + 1, mod_->getSize());

    for (int i = 0; i < nbSamples_; i++) {
        mod_->asset(path, t, opt_->getMaturity(), opt_->getTimeSteps(), rng_, past);
        for (int d = 0; d < mod_->getSize(); d++) {
            mod_->shiftAsset(shift_path, path, d, fdStep_, t, timestep);
            double tmp = opt_->payoff(shift_path);
            mod_->shiftAsset(shift_path, path, d, -fdStep_, t, timestep);
            tmp -= opt_->payoff(shift_path);
            pnl_vect_set(delta, d, pnl_vect_get(delta, d) + tmp);
            pnl_vect_set(var, d, pnl_vect_get(var, d) + SQR(tmp));
        }
    }
    double factor = 2 * fdStep_ * exp(mod_->getR() * (opt_->getMaturity() - t));
    for (int d = 0; d < mod_->getSize(); d++) {
        pnl_vect_set(delta, d, pnl_vect_get(delta, d) / (factor * nbSamples_ * pnl_mat_get(past, past->m - 1, d)));
        pnl_vect_set(var, d, pnl_vect_get(var, d) / (SQR(factor) * nbSamples_ * SQR(pnl_mat_get(past, past->m - 1, d))) - SQR(pnl_vect_get(delta, d)));
        pnl_vect_set(ic, d, sqrt(pnl_vect_get(var, d) / (double)nbSamples_));
    }
    //free
    pnl_vect_free(&var);
    pnl_mat_free(&path);
    pnl_mat_free(&shift_path);
}

/**
 * Calcule le P&L le long d'une trajectoire de marché
 *
 * @param[in] path contient la trajectoire de marché
 * @param[out] prix contient le prix
 * @param[out] ic contient la largeur de l'intervalle
 * de confiance sur le calcul du prix
 * @param[out] error contient l'erreur de couverture
 */
void MonteCarlo::profitAndLoss(const PnlMat *path, double &prix, double &icPrix, double &error) {

    int H = path->m -1;
    double timeStep = opt_->getMaturity() / (double)H;
    PnlVect *spots = pnl_vect_create(opt_->getSize());
    PnlVect *delta = pnl_vect_create(opt_->getSize());
    PnlVect *varDelta = pnl_vect_new();
    PnlVect *icDelta = pnl_vect_create(opt_->getSize());
    PnlMat *past = pnl_mat_create(1, opt_->getSize());

    pnl_mat_get_row(spots, path, 0);
    pnl_mat_set_row(past, spots, 0);
    MonteCarlo::price(past, 0, prix, icPrix);
    MonteCarlo::delta(past, 0, delta, icDelta);
    double delta0 = prix - pnl_vect_scalar_prod(delta, spots);

    for (int i = 1; i <= H; i++) {
        pnl_mat_get_row(spots, path, i);
        if (floor((i - 1) * opt_->getTimeSteps() / (double)H) < (i - 1) * opt_->getTimeSteps() / (double)H) {
            pnl_mat_set_row(past, spots, past->m - 1);
        } else {
            pnl_mat_add_row(past, past->m, spots);
        }

        pnl_vect_clone(varDelta, delta);
        MonteCarlo::delta(past, i * timeStep, delta, icDelta);
        pnl_vect_minus_vect(varDelta, delta);

        delta0 = delta0 * exp(mod_->getR() * timeStep) + pnl_vect_scalar_prod(varDelta, spots);
    }

    error = delta0 + pnl_vect_scalar_prod(delta, spots) - opt_->payoff(past);

   pnl_vect_free(&spots);
   pnl_vect_free(&delta);
   pnl_vect_free(&varDelta);
   pnl_vect_free(&icDelta);
   pnl_mat_free(&past);
}
