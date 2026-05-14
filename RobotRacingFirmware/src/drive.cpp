#include "drive.hpp"

#define LEFT_SERVO_PIN 14
#define RIGHT_SERVO_PIN 15

#define MAX_PULSE_WIDTH 2000
#define MIN_PULSE_WIDTH 1000
#define STOP_SPEED 0


void Drive::init(int timeout) {
    this->commandTimeout = timeout;
    this->lastCommand = millis();

    leftServo.attach(LEFT_SERVO_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    rightServo.attach(RIGHT_SERVO_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

    xTaskCreate(
        watchdogFunction,
        "drive_watchdog",
        2048,
        this,
        1,
        &watchdogTask
    );
}

void Drive::setSpeed(int left, int right) {
    int leftPulse = map(left, -100, 100, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    int rightPulse = map(right, -100, 100, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

    leftServo.writeMicroseconds(leftPulse);
    rightServo.writeMicroseconds(rightPulse);

    this->lastCommand = millis();
}

bool Drive::isTimeout() {
    return millis() - lastCommand > commandTimeout;
}

static void watchdogFunction(void *param){
    Drive *drive = static_cast<Drive *>(param);
    while (true)
    {
        if (drive->isTimeout())
        {
            drive->setSpeed(STOP_SPEED, STOP_SPEED);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}