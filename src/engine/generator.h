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
    void scale(double xScale, double yScale) { m_xScale = xScale; m_yScale = yScale; }

private:
    Functions m_functions;

    double m_xScale;
    double m_yScale;

    double S;        // Stock price
    double K;        // Strike price
    double r;        // Risk-free rate (annual)
    double q;        // Dividend yield (annual)
    double sigma;    // Volatility
    double T;        // Time in years
    double Z;        // Standard normal value
};

#endif // GENERATOR_H
