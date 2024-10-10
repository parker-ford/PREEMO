//#include "PreemoRoot.h"
#include "Preemo.h"
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

    preemo::Scene scene;
    preemo::Entity ent = scene.CreateEntity();
    scene.CreateEntity();
    scene.CreateEntity();
    scene.CreateEntity();



    //auto component = ent.AddComponent<preemo::TransformComponent>();
    //auto component = ent.AddComponent("Transformcomponent");


    /*ent.AddComponent("TransformComponent");
    if (ent.HasComponent("TransformComponent")) {
        std::cout << "found transform " << std::endl;
    }
    else {
        std::cout << "Did not find transform" << std::endl;
    }*/





    ///---

    class Controller : public preemo::ScriptableEntity {
    public:
        void OnCreate() {
            std::cout << "CONTROLLER CREATED!" << std::endl;
        }
        void OnUpdate() {
            std::cout << "UPDATE" << std::endl;
            if (HasComponent<preemo::TransformComponent>()) {
                std::cout << "HAS" << std::endl;
            }
        }
        
    };

    class Controller2 : public preemo::ScriptableEntity {
    public:
        void OnUpdate() {
            std::cout << "UPDATE 2" << std::endl;
        }
    };

    //ent.AddComponent<preemo::NativeScriptComponent>().Bind<Controller>();
    //ent.AddComponent<preemo::NativeScriptComponent>().Bind<Controller2>();

    root.Run(&scene);
    
    glfwTerminate();

    return 0;
}