#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "engine/functions.h"

constexpr int SAMPLES = 1000000;

int main() {
    if (!glfwInit()) {
        exit(-1);
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Monte Carlo Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(-1);
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
