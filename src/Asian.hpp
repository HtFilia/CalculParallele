/**
 * \file Asian.hpp
 *
 * \brief Fichier header de la classe Asian
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#ifndef PROJETMODPRO_ASIAN_H
#define PROJETMODPRO_ASIAN_H

#include <jlparser/parser.hpp>

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"

/**
 * \class Asian
 *
 * \brief Classe Asian représentant une Option de type Asiatique.
 *
 */
class Asian: public Option {

private:

    /**
     * \brief K_ représente le strike de l'option.
     *
     */
    double K_;

    /**
     * \brief lambda_ représente le vecteur des coefficients de l'option.
     *
     */
    PnlVect *lambda_;

public:

    /**
     * \brief Constructeur du Basket Option.
     *
     */
    Asian(double strike, double maturity, int size, int nbTimeSteps, PnlVect* lambda);

    /**
    * \brief Constructeur d'une Option Asiatique à partir d'un fichier
    *        et à l'aide d'un Parser.
    *
    * \param[in] Le fichier contenant les paramètres de l'option Asiatique.
    *
    */
    Asian(const char *InputFile);

    /**
    * \brief Destructeur de la classe Asian.
    *
    */
    ~Asian();

    void setLambda(PnlVect *lambda);

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

#endif //PROJETMODPRO_ASIAN_H
