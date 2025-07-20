#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>


#include "gpio.h"
#include "font.h"

// SPI define
#define SPI_BITS_PER_WORD           8
#define SPI_SPEED                   1000000

//  Resolution of LCD
#define ILI9341_WIDTH               320
#define ILI9341_HEIGHT 			    240
#define ILI9341_MADCTL_MY 	        0x80
#define ILI9341_MADCTL_MX 			0x40
#define ILI9341_MADCTL_MV 			0x20
#define ILI9341_MADCTL_ML 			0x10
#define ILI9341_MADCTL_RGB 			0x00
#define ILI9341_MADCTL_BGR 			0x08
#define ILI9341_MADCTL_MH 			0x04
#define ILI9341_ROTATION 			(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)
#define ILI9341_LANDSCAPE_90 	    (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
#define ILI9341_BLACK 			    0x0000
#define ILI9341_BLUE 			    0x001F
#define ILI9341_RED 			    0xF800
#define ILI9341_GREEN 			    0x07E0
#define ILI9341_CYAN 			    0x07FF
#define ILI9341_MAGENTA 			0xF81F
#define ILI9341_YELLOW 			    0xFFE0
#define ILI9341_WHITE 			    0xFFFF

class ILI9341 {
private:
    int dc_pin;
    int reset_pin;
    int chip_id;
    int resolution_x;
    int resolution_y;
    int display_rotation;

    GPIOCtrl *dc_pin_ctrl;
    GPIOCtrl *reset_pin_ctrl;
    int spi_file = -1;

    int hardware_config();
    int spi_transfer(uint8_t *tx, uint8_t *rx, uint32_t len);
    int spi_write_cmd(uint8_t cmd);
    int spi_write_data(uint8_t *data, uint32_t len);
    int set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    int draw_pixel(uint16_t x, uint16_t y, uint16_t color);
    int fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    int write_char(uint16_t x, uint16_t y, uint16_t char_code, const font_t& font, uint16_t color, uint16_t bgcolor);
    int invert_color(bool invert);

public:
    ILI9341(int dc_pin, int reset_pin, int chip_id, int resolution_x, int resolution_y, int display_rotation);
    ~ILI9341();

    int init_LCD();
    int fill_screen(uint16_t color);
    int write_string(uint16_t x, uint16_t y, const char* str, const font_t& font, uint16_t color, uint16_t bgcolor);
    int draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* image_data_16bit);
};
