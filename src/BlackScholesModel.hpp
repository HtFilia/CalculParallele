/**
 * \file BlackScholesModel.hpp
 *
 *
 * \brief Fichier header de la classe BlackScholesModel
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#ifndef PROJETMODPRO_BLACKSCHOLESMODEL_H
#define PROJETMODPRO_BLACKSCHOLESMODEL_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

/**
 * \class BlackScholesModel
 *
 * \brief Classe représentant le modèle de Black & Scholes utilisé.
 *
 */
class BlackScholesModel {

public:

    int size_; /// correspond au nombre d'actifs du modèle.
    double r_; /// correspond au taux d'intérêt du zéro-coupon.
    double rho_; /// correspond au paramètre de corrélation du modèle.
    PnlVect *sigma_; /// correspond au vecteur de volatilités.
    PnlVect *spot_; /// correspond au vecteur de valeurs initiales des sous-jacents.
    PnlMat *gammaCholesky; /// Cholesky matrix of the correlation matrix
    PnlVect *scalarProductColumn; /// to store the column for the scalar product
    PnlVect *computationsRSigma; /// to compute r-sigma*sigma/2

    /**
     * \brief Constructeur du BSModel.
     *
     */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot);

    ~BlackScholesModel();

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
    void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);
};


#endif //PROJETMODPRO_BLACKSCHOLESMODEL_H
