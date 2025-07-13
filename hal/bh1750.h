#ifndef _BH1750_H_
#define _BH1750_H_

#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <time.h>
#include "../global_variable.h"

using namespace std; 


class BH1750 {
public:
    explicit BH1750(int address = BH1750_ADDR_1);
    ~BH1750();
    int set_resolution_mode(int cmd = BH1750_H_RESOLUTION);
    int read_light_level();

private:
    string i2c_path = "/dev/i2c-1";
    int address = 0;
    int i2c_file = -1;
};

#endif