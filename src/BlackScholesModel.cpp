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

/**
 * Génère une trajectoire du modèle et la stocke dans path
 *
 * @param[out] path contient une trajectoire du modèle.
 * C'est une matrice de taille (nbTimeSteps+1) x d
 * @param[in] T  maturité
 * @param[in] nbTimeSteps nombre de dates de constatation
 */
void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {
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