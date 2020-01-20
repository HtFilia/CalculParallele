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

private:

    /**
     * \brief pointeur vers le modèle utilisé.
     *
     */
    BlackScholesModel *mod_;

    /**
     * \brief pointeur vers l'option utilisée.
     *
     */
    Option *opt_; /*! pointeur sur l'option */

    /**
     * \brief pointeur vers le générateur aléatoire utilisé.
     *
     */
    PnlRng *rng_; /*! pointeur sur le générateur */

    /**
     * \brief pas de différence finie.
     *
     */
    double fdStep_; /*! pas de différence finie */

    /**
     * \brief nombre de tirages Monte Carlo souhaité.
     *
     */
    int nbSamples_; /*! nombre de tirages Monte Carlo */


public:


    /**
     * \brief Getter de l'attribut \refitem mod_
     *
     * @return le pointeur vers le modèle utilisé, \refitem mod_
     *
     */
    BlackScholesModel* getMod() {
        return mod_;
    }

    /**
     * \brief Getter de l'attribut \refitem opt_
     *
     * @return le pointeur vers l'option utilisée, \refitem opt_
     *
     */
    Option* getOption() {
        return opt_;
    }

    /**
    * \brief Getter de l'attribut \refitem rng_
    *
    * @return le pointeur vers générateur aléatoire utilisé, \refitem rng_
    *
    */
    PnlRng* getRng() {
        return rng_;
    }

    /**
     * \brief Getter de l'attribut \refitem fdStep_
     *
     * @return le pas de différence finie, \refitem fdStep_
     *
     */
    double getStep() {
        return fdStep_;
    }

    /**
     * \brief Getter de l'attribut \refitem nbSamples_
     *
     * @return le nombre de tirages Monte Carlo souhaité, \refitem nbSamples_
     *
     */
    int getSampleSize() {
        return nbSamples_;
    }

    /**
     * \brief Setter de l'attribut \refitem mod_
     *
     * @param[in] newModel le nouveau modèle utilisé.
     *
     */
    void setMod(BlackScholesModel* newModel) {
        mod_ = newModel;
    }

    /**
     * \brief Setter de l'attribut \refitem opt_
     *
     * @param[in] newOption la nouvelle option utilisée.
     *
     */
    void setOption(Option* newOption) {
        opt_ = newOption;
    }

    /**
     * \brief Setter de l'attribut \refitem rng_
     *
     * @param[in] newRng le nouveau générateur aléatoire utilisé.
     *
     */
    void setRng(PnlRng* newRng) {
        rng_ = newRng;
    }

    /**
     * \brief Setter de l'attribut \refitem fdStep_
     *
     * @param[in] newStep le nouveau pas de différence finie, \refitem fdStep_
     *
     */
    void setStep(double newStep) {
        fdStep_ = newStep;
    }

    /**
     * \brief Setter de l'attribut \refitem nbSamples_
     *
     * @param[in] newSampleSize le nouveau nombre de tirages Monte Carlo souhaité, \refitem nbSamples_
     */
    void setSampleSize(int newSampleSize) {
        nbSamples_ = newSampleSize;
    }

    MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples) {
        this->mod_ = mod;
        this->opt_ = opt;
        this->rng_ = rng;
        this->fdStep_ = fdStep;
        this->nbSamples_ = nbSamples;
    }

    ~MonteCarlo(){}

    /**
    * Calcule le prix de l'option à la date 0
    *
    * @param[out] prix valeur de l'estimateur Monte Carlo
    * @param[out] ic largeur de l'intervalle de confiance
    */
    void price(double &prix, double &ic);

    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void price(const PnlMat *past, double t, double &prix, double &ic);

    /**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du delta
     */
    void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);

    /**
     * Calcule le P&L le long d'une trajectoire de marché
     *
     * @param[in] path contient la trajectoire de marché
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     * @param[out] error contient l'erreur de couverture
     */
    void profitAndLoss(const PnlMat *path, double &prix, double &ic, double &error);
};


#endif //PROJETMODPRO_MONTECARLO_H
