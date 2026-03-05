#include <glad/glad.h>
#include "application.h"
#include "renderer.h"
#include "../engine/generator.h"

Application::Application(int width, int height, const char* title)
    : m_width(width), m_height(height)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application::~Application() {
    glfwTerminate();
}

void Application::run() {

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
