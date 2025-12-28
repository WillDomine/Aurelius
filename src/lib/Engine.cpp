#include "../include/Engine.h"
#include <iostream>
#include <iomanip> 

void Engine::run() {
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
        commandService.drawFrame();

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

    std::cout << "\n\nSHUTTING DOWN..." << std::endl;
}