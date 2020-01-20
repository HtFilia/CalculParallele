/**
 * \file testPayoff.cpp
 *
 * \brief Fichier test de la méthode Payoff pour différentes options.
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 13.09.2019
 *
 */


#include <iostream>
#include <stdio.h>

#include "../BlackScholesModel.hpp"
#include "../Basket.hpp"
#include "../Asian.hpp"
#include "../Performance.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char *argv[]) {

    // Hardcoded Parameters
    int N = 100;
    int d = 5;
    double T = 0.5;
    int K = 100;
    double r = 0.5;
    double rho = 0;
    PnlVect* lambda = pnl_vect_create_from_scalar(d, 0.2);
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.1);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);

    // RNG Init
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    // Market simulation
    BlackScholesModel* bsModel = new BlackScholesModel(d, r, rho, sigma, spot);
    PnlMat* path = pnl_mat_create(N+1, d);
    bsModel->asset(path, T, N, rng);

    // Options Init
    Basket* basketOption = new Basket(K, T, d, N, lambda);
    Asian* asianOption = new Asian(K, T, d, N, lambda);
    Performance* performanceOption = new Performance(T, d, N, lambda);

    // Print payoffs
    std::cout << "Payoff du basket : " << basketOption->payoff(path) << std::endl;
    std::cout << "Payoff de l'asian : " << asianOption->payoff(path) << std::endl;
    std::cout << "Payoff du performance : " << performanceOption->payoff(path) << std::endl;

    //free
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&lambda);
    pnl_rng_free(&rng);
    lambda = nullptr;
    basketOption->setLambda(NULL);
    asianOption->setLambda(NULL);
    performanceOption->setLambda(NULL);
    delete basketOption;
    delete asianOption;
    delete performanceOption;
    delete bsModel;
    pnl_mat_free(&path);

    return 0;
}
