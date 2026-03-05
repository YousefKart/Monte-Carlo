#ifndef LINE_H
#define LINE_H

#include <array>
#include "../../engine/vec2.h"
#include "../../engine/vec4.h"

class Line {
public:
    Line(float x1, float y1, float x2, float y2);

    void setColor(const Vec4<float>& color) { m_color = color; };

    const Vec2<float>* vertices() const { return m_vertices.data(); };
    const Vec4<float>& color() const { return m_color; }

private:
    std::array<Vec2<float>, 2> m_vertices;
    Vec4<float> m_color;

};

#endif // LINE_H
