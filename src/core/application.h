#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();

    void run();

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
};

#endif // APPLICATION_H
