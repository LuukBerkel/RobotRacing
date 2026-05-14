#pragma once
#include "camera.hpp"

class HTTPServer
{
private:
    Camera camera;
public:
    void init(Camera camera);
    void startServer();
};
