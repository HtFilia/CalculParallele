/**
 * \file Option.hpp
 *
 * \brief Fichier header de la classe Option
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */
#ifndef PROJETMODPRO_OPTION_H
#define PROJETMODPRO_OPTION_H

#include <ostream>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/**
 * \class Option
 *
 * \brief Classe représentant une Option quelque soit son type.
 *
 */
class Option {


public:
    double T_; /// représente la maturité de l'option.
    int size_; /// représente la dimension du modèle (redondant avec BlackScholesModel).
    int nbTimeSteps_; /// représente le nombre de pas de temps de discrétisation.


    /**
     * \brief payoff calcule la valeur du payoff sur la trajectoire.
     *
     * \param[in] path est une matrice de taille (N+1) x D contenant une trajectoire du modèle telle que créée par la fonction asset.
     *
     * \return le payoff de l'option (c'est le résultat de la fonction phi appliqué à la trajectoire).
     */
    virtual double payoff(const PnlMat *path) = 0;

};



#endif //PROJETMODPRO_OPTION_H
