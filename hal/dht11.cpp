#include "dht11.h"

Dht11Sensor::Dht11Sensor(int gpio_number)
{
    this->gpio_number = gpio_number;
    gpio = new GPIOCtrl(this->gpio_number, "dht11-pin");
}

Dht11Sensor::~Dht11Sensor()
{
    delete gpio;
}

uint64_t Dht11Sensor::micros() 
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
}

bool Dht11Sensor::wait_for_level(int level, uint64_t timeout_us) 
{
    uint64_t start = micros();
    while (gpio->get_value() != level) {
        if (micros() - start > timeout_us) 
            return false;
    }
    return true;
}

int Dht11Sensor::read(int& temperature, int& humidity) 
{
    vector<bool> bits;

    // Step 1: Send start signal
    gpio->set_dir_output();
    gpio->set_value(0);
    usleep(20000);
    gpio->set_value(1);
    usleep(30);

    // Step 2: Wait for DHT11 response
    gpio->set_dir_input();
    if (!wait_for_level(0, 200)) 
        return -1;
    if (!wait_for_level(1, 200)) 
        return -2;
    if (!wait_for_level(0, 200)) 
        return -3;

    // Step 3: Read 40 bits
    for (int i = 0; i < 40; ++i) {
        if (!wait_for_level(1, 200)) 
            return -4;
        uint64_t start = micros();
        if (!wait_for_level(0, 1000)) 
            return -5;
        uint64_t duration = micros() - start;
        bits.push_back(duration > 40); // bit 1 if HIGH > 40us
    }

    if (bits.size() != 40) 
        return -6;

    uint8_t data[5] = {0};
    for (int i = 0; i < 40; ++i) {
        data[i / 8] <<= 1;
        data[i / 8] |= bits[i] ? 1 : 0;
    }

    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) 
        return -7;

    humidity = data[0];
    temperature = data[2];
    return 0;
}
