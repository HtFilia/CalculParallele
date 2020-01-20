/**
 * \file Basket.hpp
 *
 * \brief Fichier header de la classe Option
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#ifndef PROJETMODPRO_BASKET_H
#define PROJETMODPRO_BASKET_H

#include <fstream>

#include "iostream"
#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_mathtools.h"
#include "jlparser/parser.hpp"

using namespace std;

/**
 * \class Basket
 *
 * \brief Classe Basket représentant une Option de type Basket.
 *
 */
class Basket: public Option {

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
	Basket(double strike, double maturity, int size, int nbTimeSteps, PnlVect* lambda);

	Basket(const char *InputFile);

    //destructuer n'a pas à libérer le lambda, vous devez libérer les pnl_vect vous memes dans les tests selon moi
	~Basket();

	/**
	* \brief Getter du strike de l'option, \refitem K_
	*
	* @return L'attribut \refitem K_
	*
	*/
    double getK() const;

	/**
	* \brief Getter des coefficients de payoff de l'option, \refitem lambda_
	*
	* @return L'attribut \refitem lambda_
	*
	*/
    PnlVect* getLambda() const;

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

	/**
	* \brief TODO
	*
	*/
    void RedirectToFile(const char *path);

    void setLambda(PnlVect *lambda);
};

#endif //PROJETMODPRO_BASKET_H
