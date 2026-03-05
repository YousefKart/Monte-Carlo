#ifndef GRAPH_H
#define GRAPH_H

#include "../../engine/vec4.h"
#include "../primitives/line.h"

class Graph {
public:
    Graph();

    void setWidth(float width) { m_width = width; }
    void setHeight(float height) { m_height = height; }
    void setGridResolution(int gridResolution) { m_gridResolution = gridResolution; }
    void setgridColor(Vec4<float> gridColor) { m_gridColor = gridColor; }
    void setAxisColor(Vec4<float> axisColor) { m_axisColor = axisColor; }

    const float width() const { return m_width; }
    const float height() const { return m_height; }
    const int gridResolution() const { return m_gridResolution; }
    const Vec4<float> gridColor() const { return m_gridColor; }
    const Vec4<float> axisColor() const { return m_axisColor; }

private:
    float m_width;
    float m_height;
    int m_gridResolution;

    Vec4<float> m_gridColor;
    Vec4<float> m_axisColor;
};

#endif // GRAPH_H
