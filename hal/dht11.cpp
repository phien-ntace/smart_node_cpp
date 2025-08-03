#include "dht11.h"

Dht11Sensor::Dht11Sensor(int gpio_number)
{
    this->gpio_number = gpio_number;
    this->gpio = new GPIOCtrl(this->gpio_number, "dht11-pin");
}

Dht11Sensor::~Dht11Sensor()
{
    delete gpio;
}

uint64_t Dht11Sensor::get_current_time_us() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

int Dht11Sensor::read(float& temperature, float& humidity) 
{
    const int timeout = 10000;
    gpio->set_dir_output(0);
    usleep(18000); 
    gpio->set_value(1);
    usleep(20); // 20us

    gpio->set_dir_input();

    // Wait dht11 pull down 80us
    long long t_start = this->get_current_time_us();
    while (gpio->get_value() == 1 && this->get_current_time_us() - t_start < 10000);
    if (gpio->get_value() == 1) {
        // std::cerr << "DHT11 not respond." << std::endl;
        // sleep(1);
        return -1;
    }

    // Read data
    int bits[40] = {0};
    for (int i = 0; i < 40; ++i) {
        // Wait high level
        t_start = this->get_current_time_us();
        while (gpio->get_value() == 0 && this->get_current_time_us() - t_start < 10000);

        // Measure high level
        long long t_high_start = this->get_current_time_us();
        while (gpio->get_value() == 1 && this->get_current_time_us() - t_high_start < 10000);
        long long t_high_end = this->get_current_time_us();
        if (t_high_end - t_high_start > 50) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
    
    // Decode
    int humidity_int = 0, humidity_dec = 0, temp_int = 0, temp_dec = 0, checksum = 0;
    for (int i = 0; i < 8; ++i) 
        humidity_int = (humidity_int << 1) | bits[i];
    for (int i = 8; i < 16; ++i) 
        humidity_dec = (humidity_dec << 1) | bits[i];
    for (int i = 16; i < 24; ++i) 
        temp_int = (temp_int << 1) | bits[i];
    for (int i = 24; i < 32; ++i) 
        temp_dec = (temp_dec << 1) | bits[i];
    for (int i = 32; i < 40; ++i) 
        checksum = (checksum << 1) | bits[i];
    int calculated_checksum = (humidity_int + humidity_dec + temp_int + temp_dec) & 0xFF;

    if (checksum == calculated_checksum) {
        if (humidity_int <= 100) {
            temperature = temp_int + convert_int_to_float(temp_dec);
            humidity = humidity_int + convert_int_to_float(humidity_dec);
        } else {
            // std::cerr << "Value is invalied" << std::endl;
            return -2;
        }
    } else {
        // std::cerr << "Checksum error! calculate: " << calculated_checksum 
        //             << ", get: " << checksum << std::endl;
        return -3;
    }

    return 0;
}
