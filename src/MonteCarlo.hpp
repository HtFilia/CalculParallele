/**
 * \file MonteCarlo.hpp
 *
 *
 * \brief Fichier header de la classe MonteCarlo.
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */


#ifndef PROJETMODPRO_MONTECARLO_H
#define PROJETMODPRO_MONTECARLO_H


#include <utility>

#include "BlackScholesModel.hpp"
#include "Option.hpp"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"

/**
 * \class MonteCarlo
 *
 * \brief Classe statique permettant de calculer la valeur d'une option.
 *
 */

class MonteCarlo {

public:
    BlackScholesModel *mod_; /// pointeur vers le modèle utilisé.
    Option *opt_; /// pointeur sur l'option
    PnlRng *rng_; /// pointeur sur le générateur
    double fdStep_; ///pas de différence finie
    PnlMat *path; /// matrice pour stocker les niveaux des spots
    int tiragesPerTurn_;

    MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int tiragesPerTurn);

    ~MonteCarlo();

    /**
    * Calcule le prix de l'option à la date 0
    *
    * @param[out] prix valeur de l'estimateur Monte Carlo
    * @param[out] ic largeur de l'intervalle de confiance
    */
    void price_master(double &prix, double &ic, int size, double precision, int &tour);
    void price_slave(int rank, int size);
};


#endif //PROJETMODPRO_MONTECARLO_H
