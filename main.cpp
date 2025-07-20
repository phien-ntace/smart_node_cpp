#include <iostream>

#include "hal/led_controller.h"
#include "hal/bh1750.h"
#include "hal/ili9341.h"
#include "hal/dht11.h"

int main(void)
{
    // ************* Test LED *************
    // LedController *led_0;
    // int led_channel = 0;
    // led_0 = new LedController(led_channel);
    // led_0->set_pwm_period(1000000);
    // led_0->set_pwm_duty(0);
    // led_0->set_enable(true);
    // for (int i = 0; i < 1000000; i += 100000)
    // {
    //     led_0->set_pwm_duty(i);
    //     sleep(1);
    // }
    // led_0->set_enable(false);

    // ************* Test BH1750 *************
    // BH1750 *bh1750_sens = new BH1750();
    // bh1750_sens->init();
    // for (int i = 0; i < 10; i++)
    // {
    //     int lux = bh1750_sens->read_light_level();
    //     if (lux < 0)
    //         cout << "Could not read bh1750" << endl;
    //     else
    //         cout << "Light level:: " << lux << endl;
    //     sleep(1);
    // }

    // ************* Test GPIO *************
    // GPIOCtrl *dc_pin_ctrl = new GPIOCtrl(18, "dc-pin");
    // GPIOCtrl *rs_pin_ctrl = new GPIOCtrl(23, "rs-pin");
    // dc_pin_ctrl->set_dir_output();
    // rs_pin_ctrl->set_dir_output();
    // while (1) {
    //     dc_pin_ctrl->set_value(1);
    //     rs_pin_ctrl->set_value(1);
    //     cout << "Onnnn" << endl;
    //     sleep(2);
    //     dc_pin_ctrl->set_value(0);
    //     rs_pin_ctrl->set_value(0);
    //     cout << "Offf" << endl;
    //     sleep(2);
    // }

    // ************* Test ILI9341 *************
    // ILI9341 *ili9341_lcd = new ILI9341(18, 23, 0, 320, 240, 0);
    // ili9341_lcd->init_LCD();
    // sleep(2);
    // cout << "Fill" << endl;
    // ili9341_lcd->fill_screen(ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 120, "Temperature: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 150, "Humid: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 180, "Light level: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 210, "LED status: ", font_11x18, ILI9341_RED, ILI9341_BLACK);

    // ili9341_lcd->write_string(200, 120, "degree C", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(180, 150, "%", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(225, 180, "Lux", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // while (1) {
    //     cout << "LCDD" << endl; 
    //     sleep(1);
    // }

    // ************* Test DHT11 *************
    Dht11Sensor *dht11_sensor = new Dht11Sensor(4);
    while (1)
    {
        int temp, humid;
        int ret = dht11_sensor->read(temp, humid);
        if (ret < 0)
            cout << "Could not read DHT11: " << ret << endl;
        else {
            cout << "Temperature: " << temp << endl;
            cout << "Humid: " << humid << endl;
        }
        sleep(1);
    }
    

    return 0;
}