//
// Created by pretrehr on 9/13/19.
//

#include <string>

#include "gtest/gtest.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../Basket.hpp"
#include "../BlackScholesModel.hpp"
#include "../Asian.hpp"
#include "../Performance.hpp"


using namespace std;

TEST(Payoff, ExampleHard)
{
    PnlVect* lambda = pnl_vect_create_from_scalar(1, 1);
    Basket* basketOption = new Basket(100, 10, 1, 1, lambda);
    PnlMat* path = pnl_mat_create_from_scalar(2, 1, 110);
    EXPECT_EQ(10,basketOption->payoff(path));
    pnl_vect_free(&lambda);
    lambda = nullptr;
    basketOption->setLambda(NULL);
    pnl_mat_free(&path);
    delete basketOption;
}

TEST(Payoff, ExampleParsingOption)
{
    string str = "../data/data-price/basket_1.dat";
    const char *infile = str.c_str();
    Basket* basketOption = new Basket(infile);
    PnlMat* path = pnl_mat_create_from_scalar(basketOption->getTimeSteps()+1, basketOption->getSize(), 2000);
    EXPECT_EQ(1900, basketOption->payoff(path));
    pnl_mat_free(&path);
    delete basketOption;
}


TEST(Display, Example)
{
    string str = "../data/data-price/basket_1.dat";
    const char *infile = str.c_str();
    Basket* basketOption = new Basket(infile);
    string strout = "../data/data-price/output.dat";
    const char *outfile = strout.c_str();
    basketOption->RedirectToFile(outfile);
    Basket* basketOption2 = new Basket(outfile);
    EXPECT_EQ(basketOption->getSize(), basketOption2->getSize());
    EXPECT_EQ(basketOption->getTimeSteps(), basketOption2->getTimeSteps());
    EXPECT_EQ(basketOption->getMaturity(), basketOption2->getMaturity());
    delete basketOption;
    delete basketOption2;
}


TEST(BlackScholes, Asset)
{
    int N = 10;
    int d = 5;
    double r = 0.1;
    double rho = 0.2;
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.1);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);
    BlackScholesModel *bsModel = new BlackScholesModel(d, r, rho, sigma, spot);

    PnlMat* path = pnl_mat_create(N+1, d);
    double T = 1;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    bsModel->asset(path, T, N, rng);

    //free
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    delete bsModel;
    pnl_mat_free(&path);
    pnl_rng_free(&rng);
}

TEST(Payoff, allOptions)
{
    int N = 100;
    int d = 5;
    double T = 0.5;
    int K = 100;
    double r = 0.5;
    double rho = 0;
    PnlVect* lambda = pnl_vect_create_from_scalar(d, 0.2);
    PnlVect *sigma = pnl_vect_create_from_scalar(d, 0.1);
    PnlVect *spot = pnl_vect_create_from_scalar(d, 100);

    // RNG Init
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    // Market simulation
    BlackScholesModel *bsModel = new BlackScholesModel(d, r, rho, sigma, spot);
    PnlMat* path = pnl_mat_create(N+1, d);
    bsModel->asset(path, T, N, rng);

    // Création des options
    Basket* basketOption = new Basket(K, T, d, N, lambda);
    Asian* asianOption = new Asian(K, T, d, N, lambda);
    Performance* performanceOption = new Performance(T, d, N, lambda);

    // Calcul et affichage des payoffs
    cout << "Payoff du basket : " << basketOption->payoff(path) << std::endl;
    cout << "Payoff de l'asian : " << asianOption->payoff(path) << std::endl;
    cout << "Payoff du performance : " << performanceOption->payoff(path) << std::endl;

    //free
    pnl_rng_free(&rng);
    pnl_mat_free(&path);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_vect_free(&lambda);
    lambda = nullptr;
    basketOption->setLambda(NULL);
    asianOption->setLambda(NULL);
    performanceOption->setLambda(NULL);
    delete basketOption;
    delete asianOption;
    delete performanceOption;
    delete bsModel;
}