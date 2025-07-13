#pragma once

#include <iostream>
#include <string>


class ILI9341
{
private:
    int dc_pin;
    int reset_pin;
    int chip_id;
    int resolution_x;
    int resolution_y;
    int display_rotation;
public:
    ILI9341(int dc_pin, int reset_pin, int chip_id, int resolution_x, int resolution_y, int display_rotation);
    ~ILI9341();
};
