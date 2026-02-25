#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawLine(float x1, float y1, float x2, float y2);

private:
unsigned int m_VAO;
unsigned int m_VBO;
unsigned int m_shader;
};

#endif // RENDERER_H
