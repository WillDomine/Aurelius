#include "../include/Engine.h"
#include <iostream>
#include <iomanip> 

void Engine::run() {

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    squareMesh = bufferService.uploadMesh(vertices, indices);

    std::cout << "---------------------------------" << std::endl;
    std::cout << "   AURELIUS ENGINE INITIALIZED   " << std::endl;
    std::cout << "---------------------------------" << std::endl;

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    //Main Loop
    while (!windowService.shouldClose()) {
        //Get Window Events
        glfwPollEvents();

        //Draw the Frame using the Command Service
        commandService.drawFrame(squareMesh);

        // 3. FPS Counter Logic
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            std::cout << "\rFPS: " << nbFrames 
                      << " | Frame Time: " << std::fixed << std::setprecision(3) << 1000.0 / double(nbFrames) << "ms" 
                      << "    " << std::flush; // \r allows overwriting the line
            nbFrames = 0;
            lastTime += 1.0;
        }
    }

    // Wait for the GPU to finish the last frame before we kill the services
    vkDeviceWaitIdle(deviceService.device());

    bufferService.destroyMesh(squareMesh);

    std::cout << "\n\nSHUTTING DOWN..." << std::endl;
}