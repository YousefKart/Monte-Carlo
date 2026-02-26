#ifndef LINE_H
#define LINE_H

#include <array>
#include "vec2.h"

class Line {
public:
    Line(float x1, float y1, float x2, float y2);

    const Vec2<float>* vertices() const { return m_vertices.data(); };

private:
    std::array<Vec2<float>, 2> m_vertices;
};

#endif // LINE_H
