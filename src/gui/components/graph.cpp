#include "graph.h"

Graph::Graph() :
    m_width(0.9f), m_height(0.7f), m_gridResolution(10),
    m_minValue(0.0), m_maxValue(1.0),
    m_gridColor({0.2, 0.2, 0.2, 0.2}),
    m_axisColor({1.0, 1.0, 1.0, 1.0})
{}
