#ifndef POLYLINE_H
#define POLYLINE_H

#include <vector>
#include "vec2.h"
#include "vec4.h"

class Polyline {
public:
    Polyline();

    void addPoint(float x, float y);
    void setColor(const Vec4<float>& color) { m_color = color; };

    const std::vector<Vec2<float>>& vertices() const { return m_vertices; }
    std::vector<Vec2<float>>& vertices() { return m_vertices; }
    size_t pointCount() const { return m_pointCount; }
    const Vec4<float>& color() const { return m_color; }

private:
    std::vector<Vec2<float>> m_vertices;
    size_t m_pointCount;
    Vec4<float> m_color;
};

#endif // POLYLINE_H
