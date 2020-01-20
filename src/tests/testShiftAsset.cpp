/**
 * \file testShiftAsset.cpp
 *
 * \brief Fichier test de la méthode shiftAsset.
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 19.09.2019
 *
 */


#include <iostream>
#include <stdio.h>
#include "../BlackScholesModel.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argv, char *argc[]) {

    // Hardcoded parameters
    int fdSteps = 10;
    int N = 10;
    int NPast = 5;
    int d = 5;
    double r = 0.05;
    double rho = 0;
    double T = 1;
    double t = T * (NPast + 0.5) / N;
    PnlVect* lambda = pnl_vect_create_from_scalar(d, 0.2);
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.1);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);

    // BSModel Init
    BlackScholesModel *bsModel = new BlackScholesModel(d, r, rho, sigma, spot);

    // Market Path Init
    PnlMat* path = pnl_mat_create(N+1, d);
    PnlMat* shiftPosPath = pnl_mat_create(N+1, d);

    // RNG Init
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 1);

    bsModel->asset(path, T, N, rng);
    bsModel->shiftAsset(shiftPosPath, path, 1, fdSteps, t, T/N);

    //
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&lambda);
    delete bsModel;
    pnl_rng_free(&rng);
    pnl_mat_free(&shiftPosPath);
    pnl_mat_free(&path);
}
