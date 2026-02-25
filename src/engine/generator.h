#ifndef GENERATOR_H
#define GENERATOR_H

#include "functions.h"
#include "../gui/primitives/polyline.h"

class Generator {
public:
    Generator();

    Polyline generatePolyline();

private:
    Functions m_functions;
};

#endif // GENERATOR_H
