#include "renderer.h"
#include <glad/glad.h>
#include <cassert>

static const char* vertexSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
)";

static const char* fragmentSrc = R"(
#version 330 core
uniform vec4 u_Color;
out vec4 FragColor;
void main() { FragColor = u_Color; }
)";

Renderer::Renderer() {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSrc, nullptr);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSrc, nullptr);
    glCompileShader(fs);

    m_shader = glCreateProgram();
    glAttachShader(m_shader, vs);
    glAttachShader(m_shader, fs);
    glLinkProgram(m_shader);

    m_colorLocation = glGetUniformLocation(m_shader, "u_Color");
    assert(m_colorLocation >= 0);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_shader);
}

void Renderer::drawLine(const Line& line) {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2<float>) * 2, line.vertices(), GL_DYNAMIC_DRAW);

    glUseProgram(m_shader);
    glUniform4f(m_colorLocation, line.color().r, line.color().g, line.color().b, line.color().a);
    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::drawPolyline(const Polyline& polyline) {
    size_t count = polyline.pointCount();

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2<float>) * count, polyline.vertices().data(), GL_DYNAMIC_DRAW);

    glUseProgram(m_shader);
    glUniform4f(m_colorLocation, polyline.color().r, polyline.color().g, polyline.color().b, polyline.color().a);
    glDrawArrays(GL_LINE_STRIP, 0, count);
}

void Renderer::drawGraph(const Graph& graph) {
    drawGrid(graph);
    drawAxis(graph);
}

void Renderer::drawGrid(const Graph& graph) {
    float x1 = -graph.width();
    float x2 = graph.width();
    float y1 = -graph.height();
    float y2 = graph.height();
    const Vec4<float> gridColor = graph.gridColor();

    float xStep = x1;
    float yStep = y1;
    int res = graph.gridResolution();

    for (int i = 0; i < res; i++) {
        xStep += 2 * graph.width() / res;
        yStep += 2 * graph.height() / res;

        Line horizontal(x1, yStep, x2, yStep);
        horizontal.setColor(gridColor);
        drawLine(horizontal);

        Line vertical(xStep, y1, xStep, y2);
        vertical.setColor(gridColor);
        drawLine(vertical);
    }
    
}

void Renderer::drawAxis(const Graph& graph) {
    float x1 = -graph.width();
    float x2 = graph.width();
    float y1 = -graph.height();
    float y2 = graph.height();
    const Vec4<float> axisColor = graph.axisColor();

    Line horizontal(x1, 0.0f, x2, 0.0f);
    horizontal.setColor(axisColor);
    drawLine(horizontal);

    Line vertical(x1, y1, x1, y2);
    vertical.setColor(axisColor);
    drawLine(vertical);
}
