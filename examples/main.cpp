#include "PreemoRoot.h"
#include <iostream>
#include <GLFW/glfw3.h>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }

    preemo::Root root;
    if (root.StartUp(window)) {
        std::cout << "PREEMO initialized successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to initialize PREEMO." << std::endl;
    }

    root.Run();
    
    glfwTerminate();

    return 0;
}