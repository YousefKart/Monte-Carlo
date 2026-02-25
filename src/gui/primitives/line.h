#ifndef LINE_H
#define LINE_H

#include <array>

class Line {
public:
    Line(float x1, float y1, float x2, float y2);

    const float* vertices() const { return m_vertices.data(); };

private:
    std::array<float, 4> m_vertices;
};

#endif // LINE_H
