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
    // Calcul du timestep
    double timestep = T / (double)nbTimeSteps;

    // Calcul de L
    PnlMat* L = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int d = 0; d < size_; d++) {
        pnl_mat_set(L, d, d, 1);
    }
    pnl_mat_chol(L);
    PnlVect* L_d = pnl_vect_create(size_);

    // Calcul de G
    PnlMat* G = pnl_mat_create(nbTimeSteps, size_);
    //TODO: PEUT ETRE PARALLELISE
    pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);
    PnlVect* G_i = pnl_vect_create(size_);

    // On copie les spots sur le marché
    pnl_mat_set_row(path, spot_, 0);

    // Pour chaque temps t_i
    for (int i = 1; i <= nbTimeSteps; i++) {
        // Récupération de G_i
        pnl_mat_get_row(G_i, G, i - 1);

        // Pour chaque spot d
        for (int d = 0; d < size_; d++) {
            // Récupération de L_d
            pnl_mat_get_row(L_d, L, d);

            // Calcul de l'élément (t_i, d) de la matrice en fonction de l'élément (t_(i-1), d)
            double exposant = (r_ - SQR(GET(sigma_, d)) / 2) * timestep + GET(sigma_, d) * sqrt(timestep) * pnl_vect_scalar_prod(L_d, G_i);
            pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * exp(exposant));
        }
    }

    //free
//    pnl_vect_free(&G_i);
//    pnl_vect_free(&L_d);
//    pnl_mat_free(&L);
//    pnl_mat_free(&G);
}