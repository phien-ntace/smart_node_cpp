/*********************************************************************
# Create a simple Node with Raspberry Pi 5
#
# Author : Phien Nguyen (Mark)
# Date   : 10 Aug 2025
*********************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

#include "hal/led_controller.h"
#include "hal/bh1750.h"
#include "hal/ili9341.h"
#include "hal/dht11.h"
#include "mqtt/mqtt_client.h"

LedController *led;
BH1750 *light_sensor;
Dht11Sensor *dht11;
ILI9341 *ili_lcd;
MQTT_Client *mqtt_node;

void myMessageHandler(const string& topic, const string& msg);
void thread_display_on_lcd(void);
void thread_send_to_mqtt(void);

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
    // cout << "Filllll" << endl;
    // ili9341_lcd->fill_screen(ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 120, "Temperature: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 150, "Humid: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 180, "Light level: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(0, 210, "LED status: ", font_11x18, ILI9341_RED, ILI9341_BLACK);

    // ili9341_lcd->write_string(200, 120, "degree C", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(180, 150, "%", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // ili9341_lcd->write_string(225, 180, "Lux", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    // while (1) {
    //     cout << "LCD" << endl; 
    //     sleep(1);
    // }

    // ************* Test DHT11 *************
    // Dht11Sensor *dht11_sensor = new Dht11Sensor(4);
    // while (1)
    // {
    //     float temp, humid;
    //     int ret = dht11_sensor->read(temp, humid);
    //     if (ret < 0)
    //         cout << "Could not read DHT11:: " << ret << endl;
    //     else {
    //         cout << "Temperature: " << temp << ", Humid: " << humid << endl;
    //     }
    //     sleep(3);
    // }

    // ************** Test MQTT *******************
    // const string SERVER_ADDRESS = "ssl://2d4140df546f4ac3aefa3c4f2dd163bc.s1.eu.hivemq.cloud:8883";
    // const string CLIENT_ID = "pi5_client";
    // const string USERNAME = "smartnode";
    // const string PASSWORD = "Smartnode123";
    // const string TOPIC_PUBLISH = "weather";
    // const string TOPIC_SUBSCRIBE = "hiveMQ";

    // MQTT_Client *mqtt_client = new MQTT_Client(SERVER_ADDRESS, CLIENT_ID, USERNAME, PASSWORD);
    // mqtt_client->setOnMessage(myMessageHandler);

    // if (mqtt_client->connect()) {
    //     mqtt_client->subscribe(TOPIC_SUBSCRIBE, 1);
    //     mqtt_client->publish(TOPIC_PUBLISH, 1, "Hello from Raspberry Pi 5!");
    // } else {
    //     cerr << "MQTT connect failed!" << endl;
    //     return 1;
    // }
    
    // while (true) {
    //     this_thread::sleep_for(chrono::seconds(1));
    // }

    // ************** Main code **************
    cout << "Simple node from Pi 5" << endl;

    // Init LED
    led = new LedController(0);
    led->set_pwm_period(1000000);
    led->set_pwm_duty(0);
    led->set_enable(true);

    // Init light sensor
    light_sensor = new BH1750();
    light_sensor->init();

    // Init DHT11
    dht11 = new Dht11Sensor(4);

    // Init ILI9341
    ili_lcd = new ILI9341(18, 23, 0, 320, 240, 0);
    ili_lcd->init_LCD();
    ili_lcd->fill_screen(ILI9341_BLACK);
    ili_lcd->write_string(0, 120, "Temperature: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(0, 150, "Humid: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(0, 180, "Light level: ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(0, 210, "LED status: ", font_11x18, ILI9341_RED, ILI9341_BLACK);

    ili_lcd->write_string(200, 120, "degree C", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(180, 150, "%", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(225, 180, "Lux", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);

    // Init MQTT
    const string SERVER_ADDRESS = "ssl://xxx.s1.eu.hivemq.cloud:8883";
    const string CLIENT_ID = "pi5_client";
    const string USERNAME = "smartnode";
    const string PASSWORD = "Smartnode123";
    // const string TOPIC_PUBLISH = "weather";
    const string TOPIC_SUBSCRIBE = "hiveMQ";

    mqtt_node = new MQTT_Client(SERVER_ADDRESS, CLIENT_ID, USERNAME, PASSWORD);
    mqtt_node->setOnMessage(myMessageHandler);

    if (mqtt_node->connect()) {
        mqtt_node->subscribe(TOPIC_SUBSCRIBE, 1);
    } else {
        cerr << "MQTT connect failed!" << endl;
        return 1;
    }

    // Create thread
    thread tid_1(thread_display_on_lcd);
    thread tid_2(thread_send_to_mqtt);
    tid_1.join();
    tid_2.join();

    return 0;
}

void myMessageHandler(const string& topic, const string& msg)
{
    cout << "MQTT - " << topic << ", Received data: " << msg << endl;
}

void lcd_update_temperature(float value) 
{
    char buf[16];
    sprintf(buf, "%.1f", value);

    ili_lcd->write_string(150, 120, "    ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(150, 120, buf, font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
}
    
void lcd_update_humid(float value)
{
    char buf[16];
    sprintf(buf, "%d", (int)value);

    ili_lcd->write_string(150, 150, "    ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(150, 150, buf, font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
}
    
void lcd_update_light_level(int value)
{
    char buf[16];
    sprintf(buf, "%d", (int)value);

    ili_lcd->write_string(150, 180, "    ", font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
    ili_lcd->write_string(150, 180, buf, font_11x18, ILI9341_MAGENTA, ILI9341_BLACK);
}

void lcd_update_led_status(bool status)
{
    char buf[16];
    if (status)
        sprintf(buf, "On");
    else
        sprintf(buf, "Off");

    ili_lcd->write_string(150, 210, "    ", font_11x18, ILI9341_RED, ILI9341_BLACK);
    ili_lcd->write_string(150, 210, buf, font_11x18, ILI9341_RED, ILI9341_BLACK);
}

void lcd_update_time(char* str)
{
    ili_lcd->write_string(220, 0, "    ", font_11x18, ILI9341_WHITE, ILI9341_BLACK);
    ili_lcd->write_string(220, 0, str, font_11x18, ILI9341_WHITE, ILI9341_BLACK);
}

int cur_light_level = 0;
float cur_temp = 0, cur_humid = 0;
bool LED_status = false;

void thread_display_on_lcd(void)
{
    while (1)
    {
        // Get light sensor
        cur_light_level = light_sensor->read_light_level();
        if (cur_light_level > 100) {
            led->set_pwm_duty(0);
            LED_status = false;
        }
        else {
            led->set_pwm_duty(500000);
            LED_status = true;
        }
        lcd_update_light_level(cur_light_level);
        lcd_update_led_status(LED_status);

        // Get dht11
        int ret = dht11->read(cur_temp, cur_humid);
        if (ret >= 0) {
            lcd_update_temperature(cur_temp);
            lcd_update_humid(cur_humid);
        }

        // Time
        auto now = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(now);
        tm local_time = *localtime(&t);

        int hour   = local_time.tm_hour;
        int minute = local_time.tm_min;
        int second = local_time.tm_sec;
        char time[16];
        sprintf(time, "%02d:%02d:%02d", hour, minute, second);
        lcd_update_time(time);  
              
        sleep(1);
    }
}

void thread_send_to_mqtt(void)
{
    const string TOPIC_PUBLISH = "weather";
    sleep(3);

    while (1)
    {
        // Time
        auto now = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(now);
        tm local_time = *localtime(&t);

        int hour   = local_time.tm_hour;
        int minute = local_time.tm_min;
        int second = local_time.tm_sec;

        char data[100];
        sprintf(data, "%02d:%02d:%02d - Light level: %d lx, Temperature: %.1f degree C, Humid: %d%%", 
            hour, minute, second, cur_light_level, cur_temp, (int)cur_humid);
        // cout << "MQTT:" << data << endl;
        mqtt_node->publish(TOPIC_PUBLISH, 1, data);

        sleep(10);
    }    
}


