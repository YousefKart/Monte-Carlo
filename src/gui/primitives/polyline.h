#ifndef POLYLINE_H
#define POLYLINE_H

#include <vector>

class Polyline {
public:
    Polyline();

    void addPoint(float x, float y);

    const float* vertices() const { return m_vertices.data(); }
    size_t pointCount() const { return m_pointCount; }

private:
    std::vector<float> m_vertices;
    size_t m_pointCount;
};

#endif // POLYLINE_H
