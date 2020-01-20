/**
 * \file BlackScholesModel.cpp
 *
 *
 * \brief Fichier source de la classe BlackScholesModel
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#include <iostream>
#include "BlackScholesModel.hpp"
#include "math.h"

double compute_r_sigma(double a, double b)
{
    return a - b * b / 2;
}

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot)
{
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = sigma;
    spot_ = spot;

    // to compute the scalar product
    scalarProductColumn = pnl_vect_create(size_);

    computationsRSigma = pnl_vect_create_from_double(size_, r);
    pnl_vect_map_vect_inplace(computationsRSigma, sigma, compute_r_sigma);

    // Single dimension BlackScholes
    if (rho == 1)
    {
        size_ = 1;
    }
    // creation of the correlation matrix
    gammaCholesky = pnl_mat_create_from_double(size_, size_, rho);
    for (int i = 0; i < size_; i++)
    {
        pnl_mat_set_diag(gammaCholesky, 1.0, i);
    }
    // calcul of the Cholesky matrix
    pnl_mat_chol(gammaCholesky);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_mat_free(&gammaCholesky);
    pnl_vect_free(&scalarProductColumn);
    pnl_vect_free(&computationsRSigma);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
    double delta_t = T / (double)nbTimeSteps;
    pnl_mat_set_row(path, spot_, 0);

    // creation of G
    PnlMat gaussian = pnl_mat_wrap_mat_rows(path, 1, nbTimeSteps);
    PnlVect row;
    pnl_mat_rng_normal(&gaussian, nbTimeSteps, size_, rng);

    for (int i = 1; i <= nbTimeSteps; i++)
    {
        pnl_mat_get_row(scalarProductColumn, path, i);
        for (int d = 0; d < size_; d++)
        {
            row = pnl_vect_wrap_mat_row(gammaCholesky, d);
            MLET(path, i, d) = MGET(path, i - 1, d) * exp(GET(computationsRSigma, d) * delta_t + GET(sigma_, d) * sqrt(delta_t) * pnl_vect_scalar_prod(&row, scalarProductColumn));
        }
    }
}