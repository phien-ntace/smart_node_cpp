#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>

#include "gpio.h"


class Dht11Sensor
{
private:
    int gpio_number;
    GPIOCtrl *gpio;

    uint64_t micros();
    bool wait_for_level(int level, uint64_t timeout_us);
public:
    Dht11Sensor(int gpio_number);
    ~Dht11Sensor();

    int read(int& temperature, int& humidity);



};

