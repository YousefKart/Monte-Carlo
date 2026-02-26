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
}

Application::~Application() {
    glfwTerminate();
}

void Application::run() {

    Renderer renderer;
    Generator generator;
    Polyline polylines[5];
    
    for (int i = 0; i < 5; i++) {
        polylines[i] = generator.generatePolyline();
        polylines[i].setColor(Vec4(0.1f * i, 0.25f * i, 0.5f, 1.0f));
    }

    while (!glfwWindowShouldClose(m_window)) {

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < 5; i++)
            renderer.drawPolyline(polylines[i]);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
