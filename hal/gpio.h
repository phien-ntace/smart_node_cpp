#pragma once

#include <gpiod.h>
#include <string>
#include <iostream>

using namespace std;
class GPIOCtrl
{
private:
    int gpio_number;
    string chip_name;
    string gpio_name;
    gpiod_chip *chip;
    gpiod_line *line;
    int current_output_state = -1;
    bool is_ready = false;

public:
    GPIOCtrl(int gpio_number, string gpio_name, string chip_name = "/dev/gpiochip0");
    ~GPIOCtrl();

    int set_dir_output(int value);
    int set_dir_input();
    int set_value(int value);
    int get_value();
};


