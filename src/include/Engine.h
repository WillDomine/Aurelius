#pragma once
#include "WindowService.h"
#include "DeviceService.h"
#include "SwapChainService.h"
#include "PipelineService.h"
#include "CommandService.h"

class Engine {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    
    void run();

private: 
    // Create the Window
    WindowService windowService{WIDTH, HEIGHT, "AURELIUS ENGINE"};
    // Initialize Vulkan Device (needs Window)
    DeviceService deviceService{windowService};
    // Create SwapChain (needs Device + Window)
    SwapChainService swapChainService{deviceService, windowService};
    // Create Pipeline (needs Device + SwapChain)
    PipelineService pipelineService{deviceService, swapChainService};
    // Setup Commands & Drawing (needs Everything)
    CommandService commandService{deviceService, swapChainService, pipelineService};
};