#include "line.h"

Line::Line(float x1, float y1, float x2, float y2) :
    m_vertices{x1, y1, x2, y2},
    m_color(1.0f, 1.0f, 1.0f, 1.0f)
{}
