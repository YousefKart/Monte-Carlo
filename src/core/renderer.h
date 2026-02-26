#ifndef RENDERER_H
#define RENDERER_H

#include "../gui/primitives/line.h"
#include "../gui/primitives/polyline.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawLine(const Line& line);
    void drawPolyline(const Polyline& Polyline);

private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_shader;
    int m_colorLocation;
};

#endif // RENDERER_H
