#include <iostream>

#include "hal/led_controller.h"
#include "hal/bh1750.h"

int main(void)
{
    LedController *led_0;
    int led_channel = 0;
    led_0 = new LedController(led_channel);
    led_0->set_pwm_period(1000000);
    led_0->set_pwm_duty(0);
    led_0->set_enable(true);
    // for (int i = 0; i < 1000000; i += 100000)
    // {
    //     led_0->set_pwm_duty(i);
    //     sleep(1);
    // }
    // led_0->set_enable(false);

    BH1750 *bh1750_sens = new BH1750();
    for (int i = 0; i < 10; i++)
    {
        int lux = bh1750_sens->read_light_level();
        if (lux < 0)
            cout << "Could not read bh1750" << endl;
        else
            cout << "Light level: " << lux << endl;
        sleep(1);
    }
    

    return 0;
}