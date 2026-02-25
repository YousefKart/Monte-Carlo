#include "renderer.h"
#include <glad/glad.h>

static const char* vertexSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
)";

static const char* fragmentSrc = R"(
#version 330 core
out vec4 FragColor;
void main() { FragColor = vec4(1,1,1,1); }
)";

Renderer::Renderer()
{
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

    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_shader);
}

void Renderer::drawLine(float x1, float y1, float x2, float y2)
{
    float vertices[] = { x1, y1, x2, y2 };

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glUseProgram(m_shader);
    glDrawArrays(GL_LINES, 0, 2);
}
