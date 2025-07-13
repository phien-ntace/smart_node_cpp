#include "bh1750.h"

BH1750::BH1750(int address)
{
    i2c_file = open(i2c_path.c_str(), O_RDWR);
    if (i2c_file < 0) 
    {
        cout << "Could not open " << i2c_path << endl;   
        return;
    }
    this->address = address;
    if (ioctl(i2c_file, I2C_SLAVE, this->address) < 0) {
        cout << "Could not set address for BH1750" << endl;
        close(i2c_file);
        return;
    }
}

BH1750::~BH1750()
{
    if (this->i2c_file >= 0)
    {
        close(i2c_file);
    }
    
}

int BH1750::set_resolution_mode(int cmd)
{
    if (this->i2c_file < 0)
    {       
        return -1;
    }
    int ret = write(i2c_file, &cmd, 1);
    return ret;
    
}

int BH1750::read_light_level()
{
    this->set_resolution_mode(BH1750_H_RESOLUTION);
    usleep(180000);

    uint8_t data[2];
    int ret = read(i2c_file, data, 2);
    if (ret != 2)
        return -1;

    int raw = data[0] * 8 + data[1];
    return static_cast<int>(raw/1.2);
}