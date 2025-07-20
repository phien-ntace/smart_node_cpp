#include "gpio.h"


GPIOCtrl::GPIOCtrl(int gpio_number, string gpio_name, string chip_name)
{
    this->gpio_number = gpio_number;
    this->gpio_name = gpio_name;
    this->chip_name = chip_name;

    this->chip = nullptr;
    this->line = nullptr;
    this->current_output_state = -1;
}

GPIOCtrl::~GPIOCtrl()
{
    if (this->line)
        gpiod_line_release(this->line);
    if (this->chip)
        gpiod_chip_close(this->chip);
}

int GPIOCtrl::set_dir_output()
{
if (this->line) {
        gpiod_line_release(this->line);
        this->line = nullptr;
    }
    if (this->chip) {
        gpiod_chip_close(this->chip);
        this->chip = nullptr;
    }

    this->chip = gpiod_chip_open(this->chip_name.c_str());
    if (!this->chip) {
        cout << "Could not open chip" << endl;
        return -1;
    }
    this->line = gpiod_chip_get_line(this->chip, this->gpio_number);
    if (!this->line) {
        cout << "Could not get line" << endl;
        gpiod_chip_close(this->chip);
        return -1;
    }
    if (gpiod_line_request_output(this->line, this->gpio_name.c_str(), 0) < 0) {
        cout << "Could not request output" << endl;
        return -1;
    }
    this->current_output_state = 0;
    return 0;
}

int GPIOCtrl::set_dir_input()
{
    if (this->line) {
        gpiod_line_release(this->line);
        this->line = nullptr;
    }
    if (this->chip) {
        gpiod_chip_close(this->chip);
        this->chip = nullptr;
    }

    this->chip = gpiod_chip_open(this->chip_name.c_str());
    if (!this->chip) {
        cout << "Could not open chip" << endl;
        return -1;
    }
    this->line = gpiod_chip_get_line(this->chip, this->gpio_number);
    if (!this->line) {
        cout << "Could not get line" << endl;
        gpiod_chip_close(this->chip);
        return -1;
    }
    if (gpiod_line_request_input(this->line, this->gpio_name.c_str()) < 0) {
        cout << "Could not request input" << endl;
        gpiod_chip_close(this->chip);
        return -1;
    }
    return 0;
}

int GPIOCtrl::set_value(int value)
{
    if (this->line) {
        gpiod_line_set_value(this->line, value ? 1 : 0);
        this->current_output_state = value;
        return 0;
    }
    return -1;
}

int GPIOCtrl::get_value()
{
    if (this->line) {
        int val = gpiod_line_get_value(this->line);
        return val;
    }
    return -1;
}