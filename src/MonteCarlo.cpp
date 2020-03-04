/**
 * \file MonteCarlo.cpp
 *
 *
 * \brief Fichier source de la classe MonteCarlo
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#include <math.h>
#include <iostream>

#include "MonteCarlo.hpp"
#include "mpi.h"

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int tiragesPerTurn)
{
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    path = pnl_mat_create(opt->nbTimeSteps_+1, mod->size_);
    tiragesPerTurn_ = tiragesPerTurn;
}

MonteCarlo::~MonteCarlo()
{
    pnl_mat_free(&path);
}

void MonteCarlo::price_master(double &prix, double &ic, int size, double precision, int &tour) {
    prix = 0;
    double var = 0;
    tour = 1;
    bool should_continue = true;

    // Initialisation des résultats des processus esclaves
    auto price_slaves = new double[size];
    auto var_slaves = new double[size];

    while(should_continue) {

        // Le processus maitre récupère les résultats des esclaves
        MPI_Gather(&prix, 1, MPI_DOUBLE, price_slaves, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(&var, 1, MPI_DOUBLE, var_slaves, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        for (int i = 1; i < size; i++) {
            prix += price_slaves[i];
            var += var_slaves[i];
        }

        // Le processus maitre calcule le prix final et l'intervalle de confiance à 95% associé
        double tmp_prix = exp(- mod_->r_ * opt_->T_) * prix / (tiragesPerTurn_ * tour);
        double tmp_var = exp(-2 * mod_->r_ * opt_->T_) * var / (tiragesPerTurn_ * tour) - SQR(tmp_prix);
        double tmp_ic = sqrt(tmp_var / (double) (tiragesPerTurn_ * tour));
        if (tmp_ic < precision) {
            should_continue = false;
            prix = tmp_prix;
            ic = tmp_ic;
        } else {
            tour++;
        }

        // Arret ou continuation des processus esclaves
        MPI_Bcast(&should_continue, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);

        // Déblocage des processus esclaves
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

void MonteCarlo::price_slave(int rank, int size) {
    double prix = 0;
    double var = 0;
    bool should_continue = true;

    while (should_continue) {

        // Boucle sur la partie gérée par ce processus esclave
        for (int i = tiragesPerTurn_ * (rank - 1) / (size - 1); i < tiragesPerTurn_ * rank / (size - 1); i++) {
            mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
            double tmp = opt_->payoff(path);
            prix += tmp;
            var += tmp * tmp;
        }

        // Retour des résultats au processus maitre
        MPI_Gather(&prix, 1, MPI_DOUBLE, nullptr, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(&var, 1, MPI_DOUBLE, nullptr, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Attente du traitement du maitre
        MPI_Barrier(MPI_COMM_WORLD);

        // Demande au maitre s'il doit continuer
        MPI_Bcast(&should_continue, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
        prix = 0;
        var = 0;
    }
}