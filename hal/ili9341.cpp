#include "ili9341.h"

ILI9341::ILI9341(int dc_pin, int reset_pin, int chip_id, int resolution_x, int resolution_y, int display_rotation)
{
    this->dc_pin = dc_pin;
    this->reset_pin = reset_pin;
    this->chip_id = chip_id;
    this->resolution_x = resolution_x;
    this->resolution_y = resolution_y;
    this->display_rotation = display_rotation;
}

ILI9341::~ILI9341()
{
    
}

int ILI9341::hardware_config()
{
    // Config DC pin
    dc_pin_ctrl = new GPIOCtrl(this->dc_pin, "dc-pin");
    dc_pin_ctrl->set_dir_output();

    // Config Reset pin
    reset_pin_ctrl = new GPIOCtrl(this->reset_pin, "reset-pin");
    reset_pin_ctrl->set_dir_output();

    // Config SPI
    string dev_spi_path = "/dev/spidev0." + to_string(this->chip_id);
    cout << "dev_spi_path:: " << dev_spi_path << endl;
    spi_file = open(dev_spi_path.c_str(), O_RDWR);
    if (spi_file < 0) {
        cout << "Could not open SPI dev" << endl;
        return -1;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = SPI_BITS_PER_WORD;
    uint32_t speed = SPI_SPEED;
    if (ioctl(spi_file, SPI_IOC_WR_MODE, &mode) < 0) {
        cout << "Could not set mode" << endl;
        return -1;
    }
    if (ioctl(spi_file, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        cout << "Could not set bits" << endl;
        return -1;
    }
    if (ioctl(spi_file, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        cout << "Could not set speed" << endl;
        return -1;
    }
    return 0;
}

int ILI9341::spi_transfer(uint8_t *tx, uint8_t *rx, uint32_t len)
{
    int sent_len = 0;
    const int MAX_CHUNK = 4096;

    while (sent_len < len) {
        int tx_len = len - sent_len;        
        if (tx_len > MAX_CHUNK)
            tx_len = MAX_CHUNK;

        struct spi_ioc_transfer tr{};
        tr.bits_per_word = SPI_BITS_PER_WORD;
        tr.tx_buf = (unsigned long)(tx+sent_len);
        if (rx != nullptr)
            tr.rx_buf = (unsigned long)(rx+sent_len);
        tr.len = tx_len;
        tr.speed_hz = SPI_SPEED;
        tr.cs_change = 0;
        tr.delay_usecs = 0;
        int ret = ioctl(spi_file, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 0) {
            cout << "spi_transfer failed" << endl;
            return ret;
        }
        else
            sent_len += tx_len;
    }
    
    return 0;
}

int ILI9341::spi_write_cmd(uint8_t cmd)
{
    dc_pin_ctrl->set_value(0);

    uint8_t tx = cmd, rx = 0;
    uint16_t len = 1;
    int ret = this->spi_transfer(&tx, nullptr, len);
    return ret;
}

int ILI9341::spi_write_data(uint8_t *data, uint32_t len)
{
    dc_pin_ctrl->set_value(1);
    int ret = this->spi_transfer(data, nullptr, len);
    return ret;
}

int ILI9341::set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Column addr set
    this->spi_write_cmd(0x2A);
    uint8_t data_column[4] = {static_cast<uint8_t>((x0 >> 8) & 0xFF), static_cast<uint8_t>(x0 & 0xFF), static_cast<uint8_t>((x1 >> 8) & 0xFF), static_cast<uint8_t>(x1 & 0xFF)};
    this->spi_write_data(data_column, 4);

    // Row addr set
    this->spi_write_cmd(0x2B);
    uint8_t data_row[4] = {static_cast<uint8_t>((y0 >> 8) & 0xFF), static_cast<uint8_t>(y0 & 0xFF), static_cast<uint8_t>((y1 >> 8) & 0xFF), static_cast<uint8_t>(y1 & 0xFF)};
    this->spi_write_data(data_row, 4);

    // Write to RAM
    this->spi_write_cmd(0x2C);
    
    return 0;
}

int ILI9341::init_LCD()
{
    this->hardware_config();
    
    reset_pin_ctrl->set_value(0);
    usleep(5000);
    reset_pin_ctrl->set_value(1);
    this->spi_write_cmd(0x01);
    usleep(1000000);

    this->spi_write_cmd(0xCB);
    uint8_t cb_data[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    this->spi_write_data(cb_data, sizeof(cb_data));

    this->spi_write_cmd(0xCF);
    uint8_t cf_data[] = {0x00, 0xC1, 0x30};
    this->spi_write_data(cf_data, sizeof(cf_data));

    this->spi_write_cmd(0xE8);
    uint8_t e8_data[] = {0x85, 0x00, 0x78};
    this->spi_write_data(e8_data, sizeof(e8_data));

    this->spi_write_cmd(0xEA);
    uint8_t ea_data[] = {0x00, 0x00};
    this->spi_write_data(ea_data, sizeof(ea_data));

    this->spi_write_cmd(0xED);
    uint8_t ed_data[] = {0x64, 0x03, 0x12, 0x81};
    this->spi_write_data(ed_data, sizeof(ed_data));

    this->spi_write_cmd(0xF7);
    uint8_t f7_data[] = {0x20};
    this->spi_write_data(f7_data, sizeof(f7_data));

    this->spi_write_cmd(0xC0);
    uint8_t c0_data[] = {0x23};
    this->spi_write_data(c0_data, sizeof(c0_data));

    this->spi_write_cmd(0xC1);
    uint8_t c1_data[] = {0x10};
    this->spi_write_data(c1_data, sizeof(c1_data));

    this->spi_write_cmd(0xC5);
    uint8_t c5_data[] = {0x3E, 0x28};
    this->spi_write_data(c5_data, sizeof(c5_data));

    this->spi_write_cmd(0xC7);
    uint8_t c7_data[] = {0x86};
    this->spi_write_data(c7_data, sizeof(c7_data));

    this->spi_write_cmd(0x36);
    uint8_t mdata[] = {0x48};
    this->spi_write_data(mdata, sizeof(mdata));

    this->spi_write_cmd(0x3A);
    uint8_t pixel_format[] = {0x55};
    this->spi_write_data(pixel_format, sizeof(pixel_format));

    this->spi_write_cmd(0xB1);
    uint8_t b1_data[] = {0x00, 0x18};
    this->spi_write_data(b1_data, sizeof(b1_data));

    this->spi_write_cmd(0xB6);
    uint8_t b6_data[] = {0x08, 0x82, 0x27};
    this->spi_write_data(b6_data, sizeof(b6_data));

    this->spi_write_cmd(0xF2);
    uint8_t f2_data[] = {0x00};
    this->spi_write_data(f2_data, sizeof(f2_data));

    this->spi_write_cmd(0x26);
    uint8_t gamma_func[] = {0x01};
    this->spi_write_data(gamma_func, sizeof(gamma_func));

    this->spi_write_cmd(0xE0);
    uint8_t gamma_pos[] = {
        0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
        0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03,
        0x0E, 0x09, 0x00
    };
    this->spi_write_data(gamma_pos, sizeof(gamma_pos));

    this->spi_write_cmd(0xE1);
    uint8_t gamma_neg[] = {
        0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
        0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C,
        0x31, 0x36, 0x0F
    };
    this->spi_write_data(gamma_neg, sizeof(gamma_neg));

    this->spi_write_cmd(0x11);
    usleep(120000);

    this->spi_write_cmd(0x29);
    this->spi_write_cmd(0x36);
    
    int lcd_rotation = ILI9341_ROTATION;
    if (this->display_rotation == 0) {
        lcd_rotation = ILI9341_LANDSCAPE_90;
    }
    uint8_t rot[] = { (uint8_t)lcd_rotation };
    this->spi_write_data(rot, sizeof(rot));

    cout << "ILI9341 init successful" << endl;
    return 0;
}

int ILI9341::draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= this->resolution_x || y >= this->resolution_y) {
        return -1;
    }
    this->set_address_window(x, y, x+1, y+1);
    uint8_t data[] = {static_cast<uint8_t>(color >> 8), static_cast<uint8_t>(color & 0xFF)};
    this->spi_write_data(data, 2);
    return 0;

}

int ILI9341::fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if (x >= this->resolution_x || y >= this->resolution_y) {
        return -1;
    }
    if (x + w - 1 >= this->resolution_x)
        w = this->resolution_x - x;
    if (y + h - 1 >= this->resolution_y)
        h = this->resolution_y - y;
    this->set_address_window(x, y, x+w-1, y+h-1);

    uint8_t color_spi[] = {static_cast<uint8_t>((color >> 8) & 0xFF), static_cast<uint8_t>(color & 0xFF)};
    uint32_t total_pixel = w*h;
    uint8_t *all_pixel_data = (uint8_t*)malloc(total_pixel*2);
    if (all_pixel_data == NULL)
        return -1;
    for (uint32_t i = 0; i < total_pixel; i+=2) {
        all_pixel_data[i] = color_spi[0];
        all_pixel_data[i+1] = color_spi[1];
    }
    this->spi_write_data(all_pixel_data, total_pixel*2);
    free(all_pixel_data);
    all_pixel_data = NULL;
    return 0;
}


int ILI9341::fill_screen(uint16_t color)
{
    return this->fill_rectangle(0, 0, this->resolution_x, this->resolution_y, color);
}

int ILI9341::write_char(uint16_t x, uint16_t y, uint16_t char_code, const font_t& font, uint16_t color, uint16_t bgcolor) 
{
    uint32_t char_index_in_data = (char_code - 32) * font.height;
    uint8_t color_on[2] = { (uint8_t)(color >> 8), (uint8_t)(color & 0xFF) };
    uint8_t color_off[2] = { (uint8_t)(bgcolor >> 8), (uint8_t)(bgcolor & 0xFF)};

    this->set_address_window(x, y, x + font.width - 1, y + font.height - 1);

    uint32_t total_pixels = font.width * font.height;
    uint8_t* pixel_data = (uint8_t*)malloc(total_pixels * 2);
    if (!pixel_data) return -1;

    uint32_t data_index = 0;
    for (uint16_t i = 0; i < font.height; i++) {
        uint16_t row = font.data[char_index_in_data + i];
        for (uint16_t j = 0; j < font.width; j++) {
            bool pixel_on = (row >> (15 - j)) & 0x01;
            pixel_data[data_index++] = pixel_on ? color_on[0] : color_off[0];
            pixel_data[data_index++] = pixel_on ? color_on[1] : color_off[1];
        }
    }

    this->spi_write_data(pixel_data, total_pixels * 2);
    free(pixel_data);
    return 0;
}

int ILI9341::write_string(uint16_t x, uint16_t y, const char* str, const font_t& font, uint16_t color, uint16_t bgcolor) 
{
    uint16_t current_x = x;
    uint16_t current_y = y;

    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (c == '\n' || (current_x + font.width > this->resolution_x)) {
            current_x = 0;
            current_y += font.height;

            if (current_y + font.height > this->resolution_y)
                break;

            if (c == ' ') 
                continue;
        }

        this->write_char(current_x, current_y, c, font, color, bgcolor);
        current_x += font.width;
    }

    return 0;
}        

int ILI9341::draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* image_data_16bit) 
{
    if (x >= this->resolution_x || y >= this->resolution_y) 
        return -1;
    if ((x + w - 1) >= this->resolution_x) 
        return -1;
    if ((y + h - 1) >= this->resolution_y) 
        return -1;

    this->set_address_window(x, y, x + w - 1, y + h - 1);

    uint32_t total_pixels = w * h;
    uint8_t* pixel_bytes = (uint8_t*)malloc(total_pixels * 2);
    if (pixel_bytes == nullptr) 
        return -1;

    for (uint32_t i = 0; i < total_pixels; i++) {
        pixel_bytes[i * 2]     = (uint8_t)(image_data_16bit[i] >> 8);
        pixel_bytes[i * 2 + 1] = (uint8_t)(image_data_16bit[i] & 0xFF);
    }

    this->spi_write_data(pixel_bytes, total_pixels * 2);
    free(pixel_bytes);

    return 0;
}

int ILI9341::invert_color(bool invert) 
{
    this->spi_write_cmd(invert ? 0x21 : 0x20);
    return 0;
}      