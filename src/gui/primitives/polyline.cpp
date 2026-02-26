#include "polyline.h"

Polyline::Polyline() : 
    m_pointCount(0),
    m_color(1.0f, 1.0f, 1.0f, 1.0f)
{}

void Polyline::addPoint(float x, float y) {
    m_vertices.emplace_back(x, y);
    m_pointCount++;
}
