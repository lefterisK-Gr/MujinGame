#pragma once

#include <map>
#include <memory>
#include <string>
#include "ICamera.h"
#include "Camera2D.h"

class CameraManager {
public:
    // Gets the single instance of CameraManager
    static CameraManager& getInstance() {
        static CameraManager instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    CameraManager() {}

    void addCamera(const std::string& name, std::shared_ptr<ICamera> camera) {
        cameras[name] = camera;
    }

    std::shared_ptr<ICamera> getCamera(const std::string& name) {
        auto it = cameras.find(name);
        if (it != cameras.end()) {
            return it->second;
        }
        return nullptr; // or a default camera2D.worldLocation if you prefer
    }

    void initializeCameras() {
        auto mainCamera = std::make_shared<Camera2D>();
        // Configure your mainCamera as needed
        CameraManager::getInstance().addCamera("main", mainCamera);

        auto hudCamera = std::make_shared<Camera2D>();
        // Configure your hudCamera as needed, usually orthographic with fixed positioning
        CameraManager::getInstance().addCamera("hud", hudCamera);
    }

private:
    std::map<std::string, std::shared_ptr<ICamera>> cameras;
};