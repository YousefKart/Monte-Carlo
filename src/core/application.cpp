#include <glad/glad.h>
#include "application.h"
#include "renderer.h"

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

    while (!glfwWindowShouldClose(m_window)) {

        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawLine(-0.5f, -0.5f, 0.5f, 0.5f);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
