#include "led_controller.h"

int write_to_file(string path_to_file, string content)
{
    ofstream file(path_to_file);
    if (file.is_open() == false)
    {
        cout << "File is not open" << endl;
        return -1;
    }
    file << content;
    file.close();
    return 0;
}

LedController::LedController(int channel)
{
    this->channel = channel;
    this->path_to_pwm_channel = this->path_to_pwm + "/pwm" + to_string(channel);
    if (filesystem::exists(path_to_pwm_channel) == false)
    {
        string path_to_export = this->path_to_pwm + "/export";
        if (write_to_file(path_to_export, to_string(channel)) < 0)
            cout << "Could not export PWM channel: " << to_string(channel);
    }
}

LedController::~LedController()
{
    this->set_enable(false);
}

int LedController::set_pwm_period(int period)
{
    if (write_to_file(this->path_to_pwm_channel + "/period", to_string(period)) != 0)
        return -1;
    return 0;
}

int LedController::set_pwm_duty(int duty)
{
    if (write_to_file(this->path_to_pwm_channel + "/duty_cycle", to_string(duty)) != 0)
        return -1;
    return 0;
}

int LedController::set_enable(bool state)
{
    string enable_value = state?"1":"0";
    if (write_to_file(this->path_to_pwm_channel + "/enable", enable_value) != 0)
        return -1;
    return 0;
}
