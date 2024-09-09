#include <gtest/gtest.h>
#include <GLFW/glfw3.h>
#include "PreemoRoot.h"

class GLFWTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(glfwInit()) << "Failed to initialize GLFW";
    }

    void TearDown() override {
        glfwTerminate();
    }
};

//TEST_F(GLFWTest, CreateWindow) {
//    GLFWwindow* window = glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
//    ASSERT_NE(window, nullptr) << "Failed to create GLFW window";
//
//    preemo::Root root;
//
//    glfwMakeContextCurrent(window);
//
//    ASSERT_TRUE(root.Initialize(window)) << "Preemo Root failed to Initialize";
//    root.Terminate();
//
//    glfwDestroyWindow(window);
//}
//
//TEST_F(GLFWTest, RunWindow) {
//    GLFWwindow* window = glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
//    ASSERT_NE(window, nullptr) << "Failed to create GLFW window";
//
//    preemo::Root root;
//
//    glfwMakeContextCurrent(window);
//
//    ASSERT_TRUE(root.Initialize(window)) << "Preemo Root failed to Initialize";
//
//    root.Run();
//
//    root.Terminate();
//
//    glfwDestroyWindow(window);
//}
