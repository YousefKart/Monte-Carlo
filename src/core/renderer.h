#ifndef RENDERER_H
#define RENDERER_H

#include "text.h"
#include "../gui/primitives/line.h"
#include "../gui/primitives/polyline.h"
#include "../gui/components/graph.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawLine(const Line& line);
    void drawPolyline(const Polyline& polyline);
    void drawGraph(const Graph& graph);
    void drawGrid(const Graph& graph);
    void drawAxis(const Graph& graph);
    void drawTicks(const Graph& graph);
    void drawAxisValues(const Graph& graph);
    void drawAxisLabels(const Graph& graph);

private:
    Text text{960, 640};

    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_shader;
    int m_colorLocation;
};

#endif // RENDERER_H
