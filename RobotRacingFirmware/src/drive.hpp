#pragma once
#include <ESP32Servo.h>

class Drive
{
private:
    Servo leftServo;
    Servo rightServo;
    TaskHandle_t watchdogTask;

    unsigned long commandTimeout;
    unsigned long lastCommand;
public:

    /** @brief Initializes the drive system by attaching the servos to their respective pins. */
    void init(int timeout);

    /**
     * @brief Sets the speed of the left and right servos. 
     * @param left Speed for the left servo (-100 to 100)
     * @param right Speed for the right servo (-100 to 100)
     */
    void setSpeed(int left, int right);

    /**
     * @brief Checks if the drive system has timed out.
     * @return true if the system is timeout, false otherwise.
     */
    bool isTimeout();
};

/**
 * @brief Watchdog function that runs in a separate task to monitor command timeouts. 
 * If the drive system has not received a command within the specified timeout period,
 * it will automatically stop the motors for safety.
 * @param param Pointer to the Drive instance (passed as void* for FreeRTOS task compatibility)
 */
static void watchdogFunction(void *param);