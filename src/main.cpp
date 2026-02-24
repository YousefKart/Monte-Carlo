#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "engine/functions.h"

constexpr int SAMPLES = 1000000;

int main() {
    Functions mathEngine;
    double Z, avg {0};

    for (int i = 0; i < SAMPLES; i++) {
        Z = mathEngine.generateStandardNormal();
        avg += mathEngine.computeST(100, 0.05, 0.0, 0.1, 1.0, Z) / SAMPLES;
    }

    std::cout << avg << std::endl;
    return 0;
}
