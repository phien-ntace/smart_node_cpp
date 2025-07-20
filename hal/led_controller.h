#ifndef _LED_CONTROLLER_H_
#define _LED_CONTROLLER_H_

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <filesystem>

using namespace std;

int write_to_file(string path_to_file, string content);

class LedController
{
private:
    int channel;
    bool is_exist = false;
    string path_to_pwm = "/sys/class/pwm/pwmchip0";
    string path_to_pwm_channel;

public:
    explicit LedController(int channel);
    ~LedController();
    int set_pwm_period(int period);
    int set_pwm_duty(int duty);
    int set_enable(bool state);
};

#endif