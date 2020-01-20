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


protected:

    /**
     * \brief T_ représente la maturité de l'option.
     *
     */
    double T_;

    /**
     * \brief optionSize représente la dimension du modèle (redondant avec BlackScholesModel).
     *
     */
    int size_;

    /**
     * \brief nbTimeSteps_ représente le nombre de pas de temps de discrétisation.
     *
     */
    int nbTimeSteps_;

public:

    /**
     * \brief Getter de l'attribut \refitem T_
     *
     * @return la maturité de l'option, \refitem T_
     *
     */
    double getMaturity() {
        return T_;
    }

    /**
     * \brief Getter de l'attribut \refitem size_
     *
     * @return la dimension du modèle, \refitem size_
     *
     */
    int getSize() {
        return size_;
    }

    /**
     * \brief Getter de l'attribut \refitem nbTimeSteps_
     *
     * @return le nombre de pas de temps discrétisé, \refitem nbTimeSteps_
     *
     */
    int getTimeSteps() {
        return nbTimeSteps_;
    }

    /**
     * \brief payoff calcule la valeur du payoff sur la trajectoire.
     *
     * \param[in] path est une matrice de taille (N+1) x D contenant une trajectoire du modèle telle que créée par la fonction asset.
     *
     * \return le payoff de l'option (c'est le résultat de la fonction phi appliqué à la trajectoire).
     */
    virtual double payoff(const PnlMat *path) = 0;

//    friend std::ostream &operator<<(std::ostream &os, const Option &option);
};
//
//std::ostream &operator<<(std::ostream &os, const Option &option) {
//    os << "T_: " << option.T_ << "\nsize_: " << option.size_ << "\nnbTimeSteps_: " << option.nbTimeSteps_;
//    return os;
//}


#endif //PROJETMODPRO_OPTION_H
