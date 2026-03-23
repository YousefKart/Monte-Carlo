#include "generator.h"

Generator::Generator() :
    m_functions(),
    m_xScale(1.0), m_yScale(1.0),
    S(50.0f), K(50.0f), r(0.05f), q(0.02f), sigma(0.1f), T(1.0f), Z(0.5)
{}

Polyline Generator::generatePolyline() {
    Polyline polyline;
    
    const int SAMPLES = 1000;
    const double delta_T = T / SAMPLES;
    const double drift = (r - q - sigma * sigma / 2.0) * delta_T;
    const double vol = sigma * std::sqrt(delta_T);
    double temp_S = S;
    
    polyline.addPoint(0.0, S * m_yScale);

    for (int i = 1; i <= SAMPLES; ++i) {
        Z = m_functions.generateStandardNormal();
        temp_S *= m_functions.computeSTI(drift, vol, Z);
        polyline.addPoint(i * delta_T * m_xScale, temp_S * m_yScale);
    }

    return polyline;
}

double Generator::generateMin() {
    return m_functions.computeST(S, r, q, -3 * sigma, T, Z) * m_yScale;
}

double Generator::generateMax() {
    return m_functions.computeST(S, r, q, 3 * sigma, T, Z) * m_yScale;
}
