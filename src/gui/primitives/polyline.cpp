#include "polyline.h"

Polyline::Polyline() : m_pointCount(0) {}

void Polyline::addPoint(float x, float y) {
    m_vertices.push_back(x);
    m_vertices.push_back(y);

    m_pointCount++;
}
