/**
 * \file hedge.cpp
 *
 * \brief Fichier source de l'application calculant le prix d'une option en 0
 *        et son portefeuille de couverture pour un data donné en input.
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 19.09.2019
 *
 */

#include <iostream>
#include <stdio.h>
#include <string>

#include "jlparser/parser.hpp"
#include "../BlackScholesModel.hpp"
#include "../Option.hpp"
#include "../Asian.hpp"
#include "../Performance.hpp"
#include "../Basket.hpp"
#include "../HedgingResults.hpp"
#include "../MonteCarlo.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char **argv) {

    // Option variables
    double T, strike, payoffCoeff;
    PnlVect *divid, *lambda;
    string type;
    int size;
    int timestepNumber;
    Option *option;

    // BSModel variables
    double r, rho;
    PnlVect *spot, *sigma;

    // MonteCarlo variables
    size_t n_samples;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    double fdSteps = 0.00000001;

    PnlMat *path = pnl_mat_create_from_file(argv[1]);

    // Parser and file to parse
    char *infile = argv[2];
    Param *P = new Parser(infile);

    // Parsing
    // Option Parsing
    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", size);
    P->extract("timestep number", timestepNumber);
    P->extract("payoff coefficients", lambda, size);

    // Spot only for basket and asian
    //TODO : améliorable comme la formule en ligne 76
    if (type == "basket" || type == "asian") {
        P->extract("strike", strike);
    }

    // BSModel Parsing
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    if (P->extract("dividend rate", divid, size, true) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }

    // MonteCarlo Parsing and variables
    P->extract("sample number", n_samples);

    // Create Option according to parameters
    if (type == "basket") {
        option = new Basket(strike, T, size, timestepNumber, lambda);
    } else if (type == "asian") {
        option = new Asian(strike, T, size, timestepNumber, lambda);
    } else if (type == "performance") {
        option = new Performance(T, size, timestepNumber, lambda);
    }

    // Create BSModel according to parameters
    BlackScholesModel *bsModel = new BlackScholesModel(size, r, rho, sigma, spot);

    // Create MonteCarlo according to parameters
    MonteCarlo *monteCarlo = new MonteCarlo(bsModel, option, rng, fdSteps, n_samples);

    // Resultat variables
    double prix = 0;
    double prix_std_dev = 0;
    double erreur_couverture = 0;

    // Get price, deltas and st.deviation
    monteCarlo->profitAndLoss(path, prix, prix_std_dev, erreur_couverture);

    // Print Results
    HedgingResults res(prix, prix_std_dev, erreur_couverture);
    std::cout << res << std::endl;

    //free
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    pnl_vect_free(&lambda);
    pnl_rng_free(&rng);
    pnl_mat_free(&path);
    delete P;
    delete option;
    delete bsModel;
    delete monteCarlo;

    return 0;
}