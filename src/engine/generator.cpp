#include "generator.h"

Generator::Generator() :
    m_functions(),
    S(0.5), K(1.0), r(0.05), q(0.02), sigma(0.1), T(1.0), Z(0.5)
{}

Polyline Generator::generatePolyline() {
    Polyline polyline;
    
    const int SAMPLES = 1000;
    const double delta_T = T / SAMPLES;
    const double drift = (r - q - sigma * sigma / 2.0) * delta_T;
    const double vol = sigma * std::sqrt(delta_T);
    double temp_S = S;
    
    polyline.addPoint(0.0, S);

    for (int i = 1; i <= SAMPLES; ++i) {
        Z = m_functions.generateStandardNormal();
        temp_S *= m_functions.computeSTI(drift, vol, Z);
        polyline.addPoint(i * delta_T, temp_S);
    }

    return polyline;
}

double Generator::generateMin() {
    return m_functions.computeST(S, r, q, -3 * sigma, T, Z);
}

double Generator::generateMax() {
    return m_functions.computeST(S, r, q, 3 * sigma, T, Z);
}
