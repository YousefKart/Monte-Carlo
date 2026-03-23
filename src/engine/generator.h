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

    // Parameter accessors for UI controls
    float& S_ref() { return S; }
    float& K_ref() { return K; }
    float& r_ref() { return r; }
    float& q_ref() { return q; }
    float& sigma_ref() { return sigma; }
    float& T_ref() { return T; }

private:
    Functions m_functions;

    double m_xScale;
    double m_yScale;

    float S; // Stock price
    float K; // Strike price
    float r; // Risk-free rate (annual)
    float q; // Dividend yield (annual)
    float sigma; // Volatility
    float T; // Time in years
    double Z; // Standard normal value
};

#endif // GENERATOR_H
