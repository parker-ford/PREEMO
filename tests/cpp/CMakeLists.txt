
if(NOT EMSCRIPTEN)

    add_executable(PREEMOTests
        test_preemo.cpp
    )

    add_executable(PREEMOTestsGLFW
        test_preemo_GLFW.cpp
    )

    target_link_libraries(PREEMOTests
        gtest_main
        PREEMO
    )

    target_link_libraries(PREEMOTestsGLFW
        gtest_main
        PREEMO
        glfw
        glfw3webgpu
    )

    include(GoogleTest)
    gtest_discover_tests(PREEMOTests)
    gtest_discover_tests(PREEMOTestsGLFW)

endif()