/**
 * \file testProfitAndLoss.cpp
 *
 * \brief Fichier test de la méthode P&L pour différentes options.
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 21.09.2019
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
    double fdSteps = 0.01;
    int M = 1000;
    int N = 150;
    int H = 450;
    int d = 2;
    double r = 0.02;
    double rho = 0;
    double T = 1.5;
    PnlVect *lambda = pnl_vect_create_from_scalar(d, 1 / (double)d);
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.1);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);

    // Option Init
    Basket *basketAverageOption = new Basket(K, T, d, N, lambda);

    // BSModel Init
    BlackScholesModel *bsModel = new BlackScholesModel(d, r, rho, sigma, spot);

    // RNG Init
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 1);

    // Monte Carlo Init
    MonteCarlo *monteCarlo = new MonteCarlo(bsModel, basketAverageOption, rng, fdSteps, M);

    // Paramètres de sortie
    double prix = 0;
    double ic = 0;
    double error = 0;

    // Market Init
    PnlMat *path = pnl_mat_create(H + 1, d);
    bsModel->asset(path, T, H, rng);
    pnl_rng_sseed(rng, time(NULL));

    // Test Couverture
    monteCarlo->profitAndLoss(path, prix, ic, error);
    std::cout << "Prix en 0 : " << prix << std::endl;
    std::cout << "icPrix en 0 : " << ic << std::endl;
    std::cout << "Error : " << error << std::endl;

    //free
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&lambda);
    lambda = nullptr;
    basketAverageOption->setLambda(NULL);
    delete bsModel;
    delete basketAverageOption;
    pnl_rng_free(&rng);
    delete monteCarlo;
    pnl_mat_free(&path);

    return 0;
}