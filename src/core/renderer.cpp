#include "renderer.h"
#include <glad/glad.h>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <numbers>

namespace {
std::string formatTickValue(double value, double step) {
    if (std::abs(value) < 1e-9) {
        value = 0.0;
    }

    int precision = 0;
    const double absStep = std::abs(step);
    if (absStep < 1.0) {
        if (absStep >= 0.1) {
            precision = 1;
        } else if (absStep >= 0.01) {
            precision = 2;
        } else {
            precision = 3;
        }
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    std::string result = out.str();

    if (precision > 0) {
        while (!result.empty() && result.back() == '0') {
            result.pop_back();
        }
        if (!result.empty() && result.back() == '.') {
            result.pop_back();
        }
    }

    return result;
}

std::string formatTimeValue(double value, int resolution) {
    if (std::abs(value) < 1e-9) {
        value = 0.0;
    }

    int precision = 1;
    if (resolution > 10) {
        precision = 2;
    }
    if (resolution > 100) {
        precision = 3;
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

float estimateTextWidthNdc(const std::string& text, float scale) {
    constexpr float avgGlyphAdvancePx = 9.0f;
    return static_cast<float>(text.size()) * avgGlyphAdvancePx * scale;
}
}

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

    text.loadFont("../fonts/Roboto-Regular.ttf", 16);
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
    drawTicks(graph);
    drawAxisValues(graph);
    drawAxisLabels(graph);
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

    Line horizontal(x1, y1, x2, y1);
    horizontal.setColor(axisColor);
    drawLine(horizontal);

    Line vertical(x1, y1, x1, y2);
    vertical.setColor(axisColor);
    drawLine(vertical);
}

void Renderer::drawTicks(const Graph& graph) {
    float x1 = -graph.width();
    float x2 = graph.width();
    float y1 = -graph.height();
    float y2 = graph.height();
    const Vec4<float> axisColor = graph.axisColor();

    float xStep = x1;
    float yStep = y1;
    int res = graph.gridResolution();

    for (int i = 0; i < res; i++) {
        xStep += 2 * graph.width() / res;
        yStep += 2 * graph.height() / res;

        Line horizontal(xStep, y1-0.025f, xStep, y1);
        horizontal.setColor(axisColor);
        drawLine(horizontal);

        Line vertical(x1-0.025f, yStep, x1, yStep);
        vertical.setColor(axisColor);
        drawLine(vertical);
    }
}

void Renderer::drawAxisValues(const Graph& graph) {
    float x1 = -graph.width();
    float y1 = -graph.height();
    float x2 = graph.width();
    float y2 = graph.height();

    const int res = graph.gridResolution();
    if (res <= 0) {
        return;
    }

    const double valueRange = graph.maxValue() - graph.minValue();
    const double xRange = x2 - x1;
    const double yRange = y2 - y1;

    const float labelScale = 0.0025f;
    const float tickLength = 0.025f;
    const float xLabelGap = 0.04f;
    const float yLabelGap = 0.01f;
    const double yStepValue = valueRange / static_cast<double>(res);
    const float xLabelY = y1 - tickLength - xLabelGap;

    Vec4<float> color = graph.axisColor();
    glm::vec3 colorVec3(color.r, color.g, color.b);

    const std::string xStartLabel = formatTimeValue(0.0, res);
    const float xStartLabelWidth = estimateTextWidthNdc(xStartLabel, labelScale);
    text.renderText(xStartLabel, x1 - (xStartLabelWidth * 0.5f), xLabelY, labelScale, colorVec3);

    float xStep = x1;
    float yStep = y1;

    for (int i = 0; i < res; i++) {
        xStep += 2 * graph.width() / res;
        yStep += 2 * graph.height() / res;

        const double xValue = ((xStep - x1) / xRange) * graph.maxTime();
        const double yValue = ((yStep - y1) / yRange) * valueRange + graph.minValue();

        const std::string xLabel = formatTimeValue(xValue, res);
        const std::string yLabel = formatTickValue(yValue, yStepValue);

        const float xLabelWidth = estimateTextWidthNdc(xLabel, labelScale);
        const float yLabelWidth = estimateTextWidthNdc(yLabel, labelScale);

        const float xLabelX = xStep - (xLabelWidth * 0.5f);

        const float yLabelX = x1 - tickLength - yLabelGap - yLabelWidth;
        const float yLabelY = yStep - 0.01f;

        text.renderText(xLabel, xLabelX, xLabelY, labelScale, colorVec3);
        text.renderText(yLabel, yLabelX, yLabelY, labelScale, colorVec3);
    }
}

void Renderer::drawAxisLabels(const Graph& graph) {
    const float x1 = -graph.width();
    const float y1 = -graph.height();
    const float x2 = graph.width();
    const float y2 = graph.height();

    const float titleScale = 0.0032f;
    const Vec4<float> color = graph.axisColor();
    const glm::vec3 colorVec3(color.r, color.g, color.b);

    const std::string xAxisLabel = "Time (years)";
    const float xAxisWidth = estimateTextWidthNdc(xAxisLabel, titleScale);
    const float xAxisX = ((x1 + x2) * 0.5f) - (xAxisWidth * 0.5f);
    const float xAxisY = y1 - 0.15f;

    const std::string yAxisLabel = "Stock price";
    const float yAxisLabelWidth = estimateTextWidthNdc(yAxisLabel, titleScale);
    const float yAxisX = x1 - 0.10f;
    const float yAxisY = (y1 + y2) * 0.5f - yAxisLabelWidth * 0.5f;

    text.renderText(xAxisLabel, xAxisX, xAxisY, titleScale, colorVec3);
    text.renderText(yAxisLabel, yAxisX, yAxisY, titleScale, colorVec3, std::numbers::pi_v<float> / 2.0f);
}