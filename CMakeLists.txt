cmake_minimum_required(VERSION 4.0)
project(AURELIUS)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.4.328.1/Include")
    set(Vulkan_LIBRARY "C:/VulkanSDK/1.4.328.1/Lib/vulkan-1.lib")
endif()

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

include_directories(${glfw_SOURCE_DIR}/include)

find_package(Vulkan REQUIRED)

add_executable(AURELIUS src/main.cpp src/struct/Engine.cpp)

target_link_libraries(AURELIUS PRIVATE Vulkan::Vulkan glfw)
target_include_directories(AURELIUS PRIVATE ${Vulkan_INCLUDE_DIRS})