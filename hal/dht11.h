#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <stdexcept>
#include <gpiod.h>

#include "gpio.h"
#include "../utility.h"


class Dht11Sensor
{
private:
    int gpio_number;
    uint64_t get_current_time_us();
    GPIOCtrl *gpio;

public:
    Dht11Sensor(int gpio_number);
    ~Dht11Sensor();
    int read(float& temperature, float& humidity);



};

