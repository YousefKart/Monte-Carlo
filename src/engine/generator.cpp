#include "generator.h"

Generator::Generator() :
    m_functions()
{}

Polyline Generator::generatePolyline() {
    Polyline polyline;

    for (int i = 0; i < 100; ++i) {
        polyline.addPoint(i / 100.0 - 0.5, m_functions.computeST(0.5, 0.05, 0.02, 0.1, 1.0, m_functions.generateStandardNormal()) - 0.5);
    }

    return polyline;
}
