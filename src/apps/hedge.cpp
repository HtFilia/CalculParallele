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
    return 0;
}