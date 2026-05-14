#pragma once
#include "camera.hpp"
#include "drive.hpp"

class HTTPServer
{
private:
    Camera *camera;
    Drive *drive;
public:
    /**
     * @brief Initializes the HTTP server with the given camera and drive instances.
     * @param camera The Camera instance to use for streaming video
     * @param drive The Drive instance to use for controlling the robot's movement
     */
    void init(Camera *camera, Drive *drive);
    void startServer();
};
