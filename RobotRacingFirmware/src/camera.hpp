#pragma once
#include <Arduino.h>
#include <esp_camera.h>

class Camera {
public:
    /**
     * @brief Initializes the camera with the specified frame size and JPEG quality. 
     * @param frame_size The desired frame size (e.g., FRAMESIZE_QVGA)
     * @param jpeg_quality The desired JPEG quality (0-63, lower is better)
     * @return esp_err_t Returns ESP_OK on success, or an error code on failure
     */
    esp_err_t init(framesize_t frame_size, int jpeg_quality);

    /**
     * @brief Captures a frame from the camera. 
     * @return camera_fb_t* Pointer to the captured frame buffer, or nullptr on failure
     */
    camera_fb_t* capture();

    /**
     * @brief Releases a previously captured frame buffer back to the camera driver. 
     * @param fb Pointer to the frame buffer to release
     */
    void release(camera_fb_t* fb);
};

