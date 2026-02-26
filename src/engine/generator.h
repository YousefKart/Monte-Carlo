#ifndef GENERATOR_H
#define GENERATOR_H

#include "functions.h"
#include "../gui/primitives/polyline.h"

class Generator {
public:
    Generator();

    Polyline generatePolyline();
    double generateMin();
    double generateMax();

private:
    Functions m_functions;

    double S;        // Stock price
    double K;        // Strike price
    double r;        // Risk-free rate (annual)
    double q;        // Dividend yield (annual)
    double sigma;    // Volatility
    double T;        // Time in years
    double Z;        // Standard normal value
};

#endif // GENERATOR_H
