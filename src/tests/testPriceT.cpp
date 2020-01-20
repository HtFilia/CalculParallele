/**
 * \file testPriceT.cpp
 *
 * \brief Fichier test de la méthode Price en t pour différentes options.
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 18.09.2019
 *
 */

#include <iostream>
#include <stdio.h>
#include "../MonteCarlo.hpp"
#include "../Basket.hpp"
#include "../Asian.hpp"
#include "../Performance.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char *argv[]) {

    // Hardcoded parameters
    double K = 100;
    int fdSteps = 2;
    int M = 10000;
    int N = 100;
    int NPast = 0;
    int d = 5;
    double r = 0.05;
    double rho = 0;
    double T = 0.5;
    double t = T * (NPast) / N;
    PnlVect* lambda = pnl_vect_create_from_scalar(d, 0.2);
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.1);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);

    // Option Init
    Basket* basketAverageOption = new Basket(K, T, d, N, lambda);

    // BSModel Init
    BlackScholesModel *bsModel = new BlackScholesModel(d, r, rho, sigma, spot);

    // RNG Init
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 1);

    // Monte Carlo Init
    MonteCarlo *monteCarlo = new MonteCarlo(bsModel, basketAverageOption, rng, fdSteps, M);
    double price = 0;
    double ic = 0;

    // Past Market Init (to t_i)
    PnlMat* past = pnl_mat_create(NPast + 2, d);
    bsModel->asset(past, t, NPast + 1, rng);
    pnl_rng_sseed(rng, time(NULL));

    // Test Pricer en t
    monteCarlo->price(past, t, price, ic);
    std::cout << "Prix en t : " << price << std::endl;
    std::cout << "Largeur intervalle Confiance : " << ic << std::endl;

    //free
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&lambda);
    pnl_rng_free(&rng);
    pnl_mat_free(&past);
    lambda = nullptr;
    basketAverageOption->setLambda(NULL);
    delete bsModel;
    delete basketAverageOption;
    delete monteCarlo;

    return 0;
}