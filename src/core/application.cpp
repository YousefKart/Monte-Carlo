#include <glad/glad.h>
#include <algorithm>
#include <limits>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

Application::~Application() {
    if (m_window != nullptr) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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

    int numPaths = 50;
    float pathOpacity = 0.35f;
    std::vector<Polyline> polylines(numPaths);

    auto regenerate = [&]() {
        generator.scale(graph.width(), graph.height());
        polylines.resize(numPaths);

        for (int i = 0; i < numPaths; i++) {
            polylines[i] = generator.generatePolyline();
            polylines[i].setColor(Vec4(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                pathOpacity
            ));
        }

        float min = std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::lowest();
        for (int i = 0; i < numPaths; i++) {
            for (const auto& v : polylines[i].vertices()) {
                if (v.y < min) min = v.y;
                if (v.y > max) max = v.y;
            }
        }
        graph.setMinValue(min);
        graph.setMaxValue(max);
        graph.setMaxTime(generator.T_ref());

        const float yRange = max - min;
        for (int i = 0; i < numPaths; i++) {
            for (auto& v : polylines[i].vertices()) {
                const float yNormalized = (yRange != 0.0f) ? (v.y - min) / yRange : 0.5f;
                v.x = 2.0f * v.x / static_cast<float>(generator.T_ref()) - static_cast<float>(graph.width());
                v.y = 2.0f * static_cast<float>(graph.height()) * yNormalized - static_cast<float>(graph.height());
            }
        }
    };

    regenerate();

    while (!glfwWindowShouldClose(m_window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawGraph(graph);
        for (int i = 0; i < numPaths; i++)
            renderer.drawPolyline(polylines[i]);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(480.0f, 220.0f), ImGuiCond_Once);
        ImGui::Begin("Generator Parameters");

        bool changed = false;
        changed |= ImGui::SliderFloat("Stock Price (S)", &generator.S_ref(), 1.0f, 500.0f);
        //changed |= ImGui::SliderFloat("Strike (K)", &generator.K_ref(), 1.0f, 500.0f);
        changed |= ImGui::SliderFloat("Risk-free (r)", &generator.r_ref(), 0.0f, 0.5f);
        changed |= ImGui::SliderFloat("Div. Yield (q)", &generator.q_ref(), 0.0f, 0.5f);
        changed |= ImGui::SliderFloat("Volatility (sigma)", &generator.sigma_ref(), 0.01f, 1.0f);
        changed |= ImGui::SliderFloat("Time (T)", &generator.T_ref(), 0.01f, 10.0f);

        ImGui::Spacing();
        changed |= ImGui::SliderInt("Num. Paths", &numPaths, 1, 200);
        changed |= ImGui::SliderFloat("Opacity", &pathOpacity, 0.0f, 1.0f);
        if (ImGui::Button("Regenerate", ImVec2(-1, 0)) || changed)
            regenerate();

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
