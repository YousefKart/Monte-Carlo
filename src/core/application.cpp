#include <glad/glad.h>
#include <algorithm>
#include "application.h"
#include "renderer.h"
#include "../engine/generator.h"

namespace {
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}
}

Application::Application(int width, int height, const char* title)
    : m_window(nullptr), m_width(width), m_height(height)
{
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = monitor ? glfwGetVideoMode(monitor) : nullptr;
    if (mode != nullptr) {
        const int maxWindowWidth = static_cast<int>(mode->width * 0.65f);
        const int maxWindowHeight = static_cast<int>(mode->height * 0.65f);

        if (m_width <= 0) {
            m_width = maxWindowWidth;
        }
        if (m_height <= 0) {
            m_height = maxWindowHeight;
        }

        m_width = std::min(m_width, maxWindowWidth);
        m_height = std::min(m_height, maxWindowHeight);
    }
    
    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        return;
    }

    if (mode != nullptr) {
        const int xPos = (mode->width - m_width) / 2;
        const int yPos = (mode->height - m_height) / 2;
        glfwSetWindowPos(m_window, xPos, yPos);
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        return;
    }

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application::~Application() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Application::run() {
    if (m_window == nullptr) {
        return;
    }

    Renderer renderer;
    Generator generator;
    Graph graph;
    Polyline polylines[50];

    generator.scale(graph.width(), graph.height());
    float min = generator.generateMin();
    float max = generator.generateMax();

    graph.setMinValue(min);
    graph.setMaxValue(max);
    
    float yRange = max - min;
    float yNormalized = 0.0f;

    for (int i = 0; i < 50; i++) {
        polylines[i] = generator.generatePolyline();
        polylines[i].setColor(Vec4(
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            0.25f
        ));
        
        for (auto& v : polylines[i].vertices()) {
            yNormalized = (yRange != 0.0f) ? (v.y - min) / yRange : 0.5f;
            v.x = 2.0 * v.x - graph.width();
            v.y = 2.0f * graph.height() * yNormalized - graph.height();
        }
    }

    while (!glfwWindowShouldClose(m_window)) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderer.drawGraph(graph);

        for (int i = 0; i < 50; i++)
            renderer.drawPolyline(polylines[i]);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
