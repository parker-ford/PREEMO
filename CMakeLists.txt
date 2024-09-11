#Top Level CMakeLists

cmake_minimum_required(VERSION 3.0...3.25)

project (
	PREEMO
	VERSION 0.1.0
	LANGUAGES CXX C
)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# Fetch and include Google Test
if(NOT EMSCRIPTEN)
	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
	)
	FetchContent_MakeAvailable(googletest)

	FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8  # Specify the version you want
    )
    FetchContent_MakeAvailable(glfw)
else()
	add_library(glfw INTERFACE)
	target_link_options(glfw INTERFACE -sUSE_GLFW=3)
endif()

# Add WebGPU
add_subdirectory(external/webgpu)

# Add glfw3webgpu subdirectory
add_subdirectory(external/glfw3webgpu)

# Use file globbing to find all .cpp files in the src directory
# file(GLOB_RECURSE PREEMO_SOURCES "src/*.cpp")

# Create the library
if(EMSCRIPTEN)
	add_executable(PREEMO 
		src/PreemoRoot.cpp
		src/PreemoRenderingManager.cpp
		src/PreemoRenderingManagerAdapter.cpp
		src/PreemoRenderingManagerDevice.cpp
		src/PreemoRenderingManagerSurface.cpp
		src/PreemoRenderingManagerInstance.cpp
		src/PreemoRenderPipeline.cpp
		src/webgpu_impl.cpp
		${CMAKE_BINARY_DIR}/intermediate/webidl_wrapper.cpp
	)
	include_directories(${CMAKE_BINARY_DIR}/intermediate)
else()
	add_library(PREEMO
		src/PreemoRoot.cpp
		src/PreemoRenderingManager.cpp
		src/PreemoRenderingManagerAdapter.cpp
		src/PreemoRenderingManagerDevice.cpp
		src/PreemoRenderingManagerSurface.cpp
		src/PreemoRenderingManagerInstance.cpp
		src/PreemoRenderPipeline.cpp
		src/webgpu_impl.cpp
	)

	# Add a new executable target for direct running
	add_executable(PREEMO_standalone
		examples/main.cpp
	)
	target_link_libraries(PREEMO_standalone 
		PREEMO
		glfw
	)
	target_include_directories(PREEMO_standalone PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/include 
		${CMAKE_CURRENT_SOURCE_DIR}/external
	)
endif()

# Set include directories
target_include_directories(PREEMO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(PREEMO PUBLIC glfw webgpu glfw3webgpu)

target_copy_webgpu_binaries(PREEMO)

set_target_properties(PREEMO PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	COMPILE_WARNING_AS_ERROR ON
)

if (MSVC)
	target_compile_options(PREEMO PRIVATE /W4)
else()
	target_compile_options(PREEMO PRIVATE -Wall -Wextra -pedantic)
endif()

if (XCODE)
	set_target_properties(PREEMO PROPERTIES
		XCODE_GENERATE_SCHEME ON
		XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal"
	)
endif()

if (EMSCRIPTEN)
	set_target_properties(PREEMO PROPERTIES SUFFIX ".js")
    target_link_options(PREEMO PRIVATE
		-sASSERTIONS=1
        -sASYNCIFY
        -sWASM=1
        -sMODULARIZE=1
        -sEXPORT_NAME="createModule"
        "--post-js=${CMAKE_BINARY_DIR}/intermediate/glue.js"
    )
    
    # set_target_properties(PREEMO PROPERTIES OUTPUT_NAME "preemo")
endif()

if(NOT EMSCRIPTEN)
	# Enable testing
	enable_testing()

	# Add the tests subdirectory
	add_subdirectory(tests)
endif()


