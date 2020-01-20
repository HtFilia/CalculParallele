/**
 * \file testSimulMarket.cpp
 *
 * \brief Fichier test de la méthode de simulation du marché.
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
    int H = 50;
    int d = 5;
    double r = 0.1;
    double rho = 0.2;
    double T = 1;
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.05);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);
    PnlVect *trend = pnl_vect_create(d);
    for (int j = 0; j < d; j++){
        pnl_vect_set(trend, j, 0.2 * j / (double)(d - 1) - 0.1);
    }

    BlackScholesModel *bsModel = new BlackScholesModel(d, r, rho, sigma, spot, trend);

    PnlMat* path = pnl_mat_create(H+1, d);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    bsModel->simul_market(path, T, H, rng);
    pnl_mat_print(path);

    //free
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&trend);
    delete bsModel;
    pnl_mat_free(&path);
    pnl_rng_free(&rng);
    return 0;
}