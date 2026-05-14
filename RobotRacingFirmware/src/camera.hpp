#pragma once
#include <Arduino.h>
#include <esp_camera.h>

class Camera {
public:
    esp_err_t init(framesize_t frame_size, int jpeg_quality);
    camera_fb_t* capture();
    void release(camera_fb_t* fb);
};

