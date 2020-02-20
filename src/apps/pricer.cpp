/**
 * \file price0.cpp
 *
 * \brief Fichier source de l'application calculant le prix d'une option en 0 et
 *        les déviations standards des estimateurs pour un data donné en input.
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
#include "../PricingResults.hpp"
#include "../MonteCarlo.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {

    // Parallelisation
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Option variables
    double T, strike, payoffCoeff;
    PnlVect *divid, *lambda;
    string type;
    int optionSize;
    int timestepNumber;
    Option *option;

    // BSModel variables
    double r, rho;
    PnlVect *spot, *sigma;

    // MonteCarlo variables
    size_t n_samples;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    double fdSteps = 0.00001;

    // Parser and file to parse
    char *infile = argv[1];
    Param *P = new Parser(infile);

    // Parsing
    // Option Parsing
    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", optionSize);
    P->extract("timestep number", timestepNumber);
    P->extract("payoff coefficients", lambda, optionSize);

    // Spot only for basket and asian
    //TODO : améliorable comme la formule en ligne 76
    if (type == "basket" || type == "asian") {
        P->extract("strike", strike);
    }

    // BSModel Parsing
    P->extract("spot", spot, optionSize);
    P->extract("volatility", sigma, optionSize);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    if (! P->extract("dividend rate", divid, optionSize, true))
    {
        divid = pnl_vect_create_from_zero(optionSize);
    }

    // MonteCarlo Parsing and variables
    P->extract("sample number", n_samples);

    // Create Option according to parameters
    if (type == "basket") {
        option = new Basket(strike, T, optionSize, timestepNumber, lambda);
    } else if (type == "asian") {
        option = new Asian(strike, T, optionSize, timestepNumber, lambda);
    } else if (type == "performance") {
        option = new Performance(T, optionSize, timestepNumber, lambda);
    }

    // Create BSModel according to parameters
    auto *bsModel = new BlackScholesModel(optionSize, r, rho, sigma, spot);

    // Create MonteCarlo according to parameters
    auto *monteCarlo = new MonteCarlo(bsModel, option, rng, fdSteps, n_samples);

    // Resultat variables
    double prix = 0;
    double prix_std_dev = 0;
    PnlVect *delta = pnl_vect_create(optionSize);
    PnlVect *delta_std_dev = pnl_vect_create(optionSize);

    // Get price, deltas and st.deviation
    monteCarlo->price(prix, prix_std_dev);

    //monteCarlo->price(prix, prix_std_dev);

    // Print Results
    std::cout << '{' << "\"price\": " << prix << ", \"priceStdDev\": " << prix_std_dev << '}' << std::endl;

    // End
    MPI_Finalize();
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    pnl_rng_free(&rng);

    delete P;
    delete option;
    delete bsModel;
    delete monteCarlo;
    return 0;
}