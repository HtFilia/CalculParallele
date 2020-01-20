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

private:

    /**
     * \brief size_ correspond au nombre d'actifs du modèle.
     *
     */
    int size_;

    /**
     * \brief r_ correspond au taux d'intérêt du zéro-coupon.
     *
     */
    double r_;

    /**
     * \brief rho_ correspond au paramètre de corrélation du modèle.
     *
     */
    double rho_;

    /**
     * \brief *sigma correspond au vecteur de volatilités.
     *
     */
    PnlVect *sigma_;

    /**
     * \brief *spot_ correspond au vecteur de valeurs initiales des sous-jacents.
     *
     */
    PnlVect *spot_;

    /**
     * \brief *trend_ correspond au vecteur des rentabilités moyennes des sous-jacents.
     *
     */
    PnlVect *trend_;

public:

    /**
     * \brief Getter de l'attribut \refitem size_
     *
     * @return l'attribut \refitem size_
     *
     */
    int getSize() {
        return size_;
    }

    /**
     * \brief Getter de l'attribut \refitem r_
     *
     * @return l'attribut \refitem r_
     *
     */
    double getR() {
        return r_;
    }

    /**
     * \brief Getter de l'attribut \refitem rho_
     *
     * @return l'attribut \refitem rho_
     *
     */
    double getRho() {
        return rho_;
    }

    /**
     * \brief Getter de l'attribut \refitem sigma_
     *
     * @return l'attribut \refitem sigma_
     *
     */
    PnlVect *getSigma() {
        return sigma_;
    }

    /**
     * \brief Getter de l'attrivut \refitem spot_
     *
     * @return l'attribut \refitem spot_
     *
     */
    PnlVect *getSpot() {
        return spot_;
    }

    /**
     * \brief Getter de l'attrivut \refitem trend_
     *
     * @return l'attribut \refitem trend_
     *
     */
    PnlVect *getTrend() {
        return trend_;
    }

    /**
     * \brief Setter de l'attribut \refitem size_
     *
     * @param newSize le nouveau nombre d'actifs du modèle, \refitem size_
     *
     */
    void setSize(int newSize) {
        size_ = newSize;
    }

    /**
     * \brief Setter de l'attribut \refitem r_
     *
     * @param newR le nouveau taux d'intérêt du ZC, \refitem r_
     */
    void setR(double newR) {
        r_ = newR;
    }

    /**
     * \brief Setter de l'attribut \refitem rho_
     *
     * @param newRho la nouvelle corrélation du modèle, \refitem rho_
     *
     */
    void setRho(double newRho) {
        rho_ = newRho;
    }

    /**
     * \brief Setter de l'attribut \refitem sigma_
     *
     * @param newSigma le nouveau vecteur de volatilités du modèle, \refitem sigma_
     *
     */
    void setSigma(PnlVect *newSigma) {
        sigma_ = newSigma;
    }

    /**
     * \brief Setter de l'attribut \refitem spot_
     *
     * @param newSpot le nouveau vecteur des spots initiaux du modèle, \refitem spot_
     *
     */
    void setSpot(PnlVect *newSpot) {
        spot_ = newSpot;
    }

    /**
     * \brief Setter de l'attribut \refitem trend_
     *
     * @param newTrend le nouveau vecteur des rentabilités du modèle, \refitem trend_
     *
     */
    void setTrend(PnlVect *newTrend) {
        trend_ = newTrend;
    }

    /**
     * \brief Constructeur du BSModel.
     *
     */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot) {
        this->size_ = size;
        this->r_ = r;
        this->rho_ = rho;
        this->sigma_ = sigma;
        this->spot_ = spot;
    }

    /**
     * \brief Constructeur du BSModel avec les rentabilités moyennes.
     *
     */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend) {
        this->size_ = size;
        this->r_ = r;
        this->rho_ = rho;
        this->sigma_ = sigma;
        this->spot_ = spot;
        this->trend_ = trend;
    }

    ~BlackScholesModel(){}

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
    void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

    /**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     */
    void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

    /**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path
     * dont la composante d a été shiftée par (1+h)
     * à partir de la date t.
     * @param[in] t date à partir de laquelle on shift
     * @param[in] h pas de différences finies
     * @param[in] d indice du sous-jacent à shifter
     * @param[in] timestep pas de constatation du sous-jacent
     */
    void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

    /**
     * Simulation du marché
     *
     * @param[out]  path contient en input la simulation du marché
     * T date jusqu'à laquelle on simule la trajectoire
     * @param[in] H nombre de dates pour la simulation
     * @param[in] rng générateur de nombres aléatoires
     */
    void simul_market(PnlMat *path, double T, int H, PnlRng *rng);
};


#endif //PROJETMODPRO_BLACKSCHOLESMODEL_H
