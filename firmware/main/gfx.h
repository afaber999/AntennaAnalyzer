#ifndef GFX_H
#define GFX_H

#include <stdio.h>
#include <inttypes.h>

#define LGFX_USE_V1
#include "LovyanGFX.hpp"


class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9341 _panel;
    lgfx::Bus_SPI _bus;
    lgfx::Light_PWM _light;
    lgfx::Touch_FT5x06 _touch;

public:
    LGFX(void)
    {
        {
            auto cfg = _bus.config();
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read = 16000000;
            cfg.spi_3wire = false;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk = 12;
            cfg.pin_mosi = 11;
            cfg.pin_miso = 13;
            cfg.pin_dc = 46;
            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _panel.config();
            cfg.pin_cs = 10;
            cfg.pin_rst = -1;
            cfg.pin_busy = -1;
            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = true;
            cfg.invert = true;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = false;
            _panel.config(cfg);
        }

        {
            auto cfg = _light.config();
            cfg.pin_bl = 45;
            cfg.invert = false;
            cfg.freq = 5000;
            cfg.pwm_channel = 0;
            _light.config(cfg);
            _panel.setLight(&_light);
        }

        {
            auto cfg = _touch.config();
            cfg.x_min = 0;
            cfg.x_max = 239;
            cfg.y_min = 0;
            cfg.y_max = 319;
            cfg.pin_int = 17;
            cfg.bus_shared = false;
            cfg.offset_rotation = 0;
            cfg.i2c_port = I2C_NUM_0;
            cfg.i2c_addr = 0x38;
            cfg.pin_sda = 16;
            cfg.pin_scl = 15;
            cfg.freq = 400000;
            cfg.pin_rst = 18;
            _touch.config(cfg);
            _panel.setTouch(&_touch);
        }

        setPanel(&_panel);
    }
};

extern LGFX gfx_lcd;

#endif