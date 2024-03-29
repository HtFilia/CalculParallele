/**
 * \file Performance.hpp
 *
 * \brief Fichier header de la classe Performance
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#ifndef PROJETMODPRO_PERFORMANCE_H
#define PROJETMODPRO_PERFORMANCE_H

#include <iostream>

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"

/**
 * \class Performance
 *
 * \brief Classe Perfomance représentant une Option de type Performance.
 *
 */

class Performance: public Option {

public:

    PnlVect *lambda_; /// représente le vecteur des coefficients de l'option.

    /**
     * \brief Constructeur du Performance Option.
     *
     */
    Performance(double maturity, int size, int nbTimeSteps, PnlVect* lambda);

    /**
    * \brief Constructeur du Performance Option à partir d'un Fichier,
    *        lecture à l'aide d'un Parser.
    *
    */
    Performance(const char *InputFile);

    /**
     * Destructeur
     */
    ~Performance();

    /**
     * \brief Calcule le payoff de l'option Basket suivant le marché qu'on lui donne.
     *
     * @param[out] path le marché contenant les spots des sous-jacents
     * 					aux différents temps étudiés.
     *
     * @return la valeur du payoff du Call.
     *
     */
    double payoff(const PnlMat *path);
};

#endif //PROJETMODPRO_PERFORMANCE_H
