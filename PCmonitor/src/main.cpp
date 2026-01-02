/*
   MIT License

  Copyright (c) 2022 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ______________  _____
  ___  __/___  /_ ___(_)_____ _______ _______
  __  /_  __  __ \__  / _  _ \__  __ `/_  __ \
  _  __/  _  /_/ /_  /  /  __/_  /_/ / / /_/ /
  /_/     /_.___/ /_/   \___/ _\__, /  \____/
                              /____/

*/

#define LGFX_USE_V1

#include <Arduino.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include "driver/uart.h"
#include "SmoothValue.h"
#include <eez-framework.h>
#include "TaskManagerIO.h"
#include "ui/ui.h"
#include "ui/screens.h"
#include "ui/actions.h"
#include "ui/images.h"
#include "HWMonitor/HwMonitor.h"

#define SCR 32
class LGFX : public lgfx::LGFX_Device
{

    lgfx::Panel_ST7796 _panel_instance;

    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

    lgfx::Touch_FT5x06 _touch_instance;

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config(); // Get the structure for bus configuration.

            // SPI bus settings
            cfg.spi_host = VSPI_HOST; // Select the SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
            // * With the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated, so if an error occurs, use SPI2_HOST and SPI3_HOST instead.
            cfg.spi_mode = 3;                  // Set SPI communication mode (0 ~ 3)
            cfg.freq_write = 40000000;         // SPI clock when sending (up to 80MHz, rounded to 80MHz divided by an integer)
            cfg.freq_read = 6000000;           // SPI clock when receiving
            cfg.spi_3wire = false;             // set to true if receiving on MOSI pin
            cfg.use_lock = true;               // set to true to use transaction lock
            cfg.dma_channel = SPI_DMA_CH_AUTO; // Set the DMA channel to use (0=not use DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=auto setting)
            // * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is recommended for the DMA channel. Specifying 1ch and 2ch is deprecated.
            cfg.pin_sclk = 14; // set SPI SCLK pin number
            cfg.pin_mosi = 13; // Set MOSI pin number for SPI
            cfg.pin_miso = -1; // set SPI MISO pin number (-1 = disable)
            cfg.pin_dc = 21;   // Set SPI D/C pin number (-1 = disable)

            _bus_instance.config(cfg);              // Apply the settings to the bus.
            _panel_instance.setBus(&_bus_instance); // Sets the bus to the panel.
        }

        {                                        // Set display panel control.
            auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

            cfg.pin_cs = 15;   // Pin number to which CS is connected (-1 = disable)
            cfg.pin_rst = 22;  // pin number where RST is connected (-1 = disable)
            cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 = disable)

            // * The following setting values ​​are set to general default values ​​for each panel, and the pin number (-1 = disable) to which BUSY is connected, so please try commenting out any unknown items.

            cfg.memory_width = 320;  // Maximum width supported by driver IC
            cfg.memory_height = 480; // Maximum height supported by driver IC
            cfg.panel_width = 320;   // actual displayable width
            cfg.panel_height = 480;  // actual displayable height
            cfg.offset_x = 0;        // Panel offset in X direction
            cfg.offset_y = 0;        // Panel offset in Y direction
            cfg.offset_rotation = 1;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = false;

            _panel_instance.config(cfg);
        }

        {                                        // Set backlight control. (delete if not necessary)
            auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

            cfg.pin_bl = 23;     // pin number to which the backlight is connected
            cfg.invert = false;  // true to invert backlight brightness
            cfg.freq = 44100;    // backlight PWM frequency
            cfg.pwm_channel = 1; // PWM channel number to use

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
        }

        { // Configure settings for touch screen control. (delete if not necessary)
            auto cfg = _touch_instance.config();

            cfg.x_min = 0;    // Minimum X value (raw value) obtained from the touchscreen
            cfg.x_max = 319;  // Maximum X value (raw value) obtained from the touchscreen
            cfg.y_min = 0;    // Minimum Y value obtained from touchscreen (raw value)
            cfg.y_max = 479;  // Maximum Y value (raw value) obtained from the touchscreen
            cfg.pin_int = 39; // pin number to which INT is connected
            cfg.bus_shared = false;
            cfg.offset_rotation = 0;

            // For I2C connection
            cfg.i2c_port = 1;    // Select I2C to use (0 or 1)
            cfg.i2c_addr = 0x38; // I2C device address number
            cfg.pin_sda = 18;    // pin number where SDA is connected
            cfg.pin_scl = 19;    // pin number to which SCL is connected
            cfg.freq = 400000;   // set I2C clock

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
        }

        setPanel(&_panel_instance); // Sets the panel to use.
    }
};

#define UART_PORT UART_NUM_0
#define UART_TX_PIN 43
#define UART_RX_PIN 44
#define UART_BAUD_RATE 115200
#define BUF_SIZE 1024
static const char *TAG = "UART_ESPNOW";

static int64_t last_packet_time = 0;
uint8_t bcast_addr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Create an instance of the prepared class.
LGFX tft;

// Define NTP Client to get time
const char *ntpServer = "europe.pool.ntp.org";
const char *timeOffset = "CET-1CEST,M3.5.0,M10.5.0/3";
static const uint32_t screenWidth = 480;
static const uint32_t screenHeight = 320;
const unsigned int lvBufferSize = screenWidth * SCR;
static lv_color_t disp_draw_buf[lvBufferSize * 2];  // Buffer for the display, must be at least as large as the screen size
static lv_color_t disp_draw_buf2[lvBufferSize * 2]; // Second buffer for the display, must be at least as large as the screen size
uint8_t lvBuffer[2][lvBufferSize];

static HWMonitor monitor;
static SmoothValueManager<32> s_smooth(0.05f);

enum Tabs
{
    ALL,
    CPU,
    GPU,
    RAM
};

/* Display flushing */
void my_disp_flush(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{

    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    lv_draw_sw_rgb565_swap(data, w * h);

    if (tft.getStartCount() == 0)
    {
        tft.endWrite();
    }
    // tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t*)data);
    tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)data);
    lv_display_flush_ready(display); /* tell lvgl that flushing is done */
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *dev, lv_indev_data_t *data)
{

    bool touched;
    uint16_t touchX, touchY;

    touched = tft.getTouch(&touchX, &touchY);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void setup()
{
    Serial.setRxBufferSize(4096);
    Serial.begin(115200);
    tft.init();
    tft.initDMA();
    tft.startWrite();
    tft.setBrightness(255);

    lv_init();

    Serial.printf("\nWidth %d\tHeight %d\n", screenWidth, screenHeight);

    if (!disp_draw_buf)
    {
        Serial.println("LVGL disp_draw_buf allocate failed!");
    }
    else
    {
        static auto *lvDisplay = lv_display_create(screenWidth, screenHeight);
        lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
        lv_display_set_flush_cb(lvDisplay, my_disp_flush);

        lv_display_set_buffers(lvDisplay, lvBuffer[0], lvBuffer[1], lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

        static auto *lvInput = lv_indev_create();
        lv_indev_set_type(lvInput, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(lvInput, my_touchpad_read);

        ui_init();

        monitor.begin();

        Serial.println("Setup done");
    }
}

void uart_espnow_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    uart_driver_install(UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    ESP_LOGI(TAG, "UART initialized (TX=%d RX=%d @ %d baud)", UART_TX_PIN, UART_RX_PIN, UART_BAUD_RATE);
}

void displayCpuDetails()
{
    lv_label_set_text_fmt(objects.label_cpu_temperature, "%.1f", s_smooth.get(0x04));
    lv_bar_set_value(objects.bar_cpu_temperature, s_smooth.get(0x04), LV_ANIM_OFF);
    lv_label_set_text_fmt(objects.label_cpu_load_core_max, "%.1f", s_smooth.get(0x02));
    lv_label_set_text_fmt(objects.label_cpu_load_total, "%.1f", s_smooth.get(0x01));
    lv_label_set_text_fmt(objects.label_cpu_power, "%.1f", s_smooth.get(0x03));
    lv_arc_set_value(objects.arc_cpu_load_core_max, s_smooth.getInt(0x02));
    lv_arc_set_value(objects.arc_cpu_load_total, s_smooth.getInt(0x01));
}

void displayGpuDetails()
{
    uint8_t hot_spot = s_smooth.getInt(0x0E);
    lv_bar_set_value(objects.bar_gpu_temp_hot_spot, hot_spot, LV_ANIM_OFF);
    lv_bar_set_start_value(objects.bar_gpu_temp_hot_spot, hot_spot - 3, LV_ANIM_OFF);
    lv_bar_set_value(objects.bar_gpu_temp_core, s_smooth.getInt(0x0D), LV_ANIM_OFF);
    lv_label_set_text_fmt(objects.label_gpu_temp_core, "%.1f", s_smooth.get(0x0D));
    lv_label_set_text_fmt(objects.label_gpu_framerate, "%d", s_smooth.getInt(0x0B) > 1000 ? 1000 : s_smooth.getInt(0x0B));
    lv_label_set_text_fmt(objects.label_gpu_power, "%.1f", s_smooth.get(0x0C));
    lv_label_set_text_fmt(objects.label_gpu_load_core, "%.1f", s_smooth.get(0x10));
    lv_arc_set_value(objects.arc_gpu_load_core, s_smooth.getInt(0x10));
    lv_label_set_text_fmt(objects.label_gpu_load_memory, "%.1f", s_smooth.get(0x11));
    lv_arc_set_value(objects.arc_gpu_load_memory, s_smooth.getInt(0x11));
}

void displayRamDetails()
{
    lv_label_set_text_fmt(objects.label_ram_used, "%.1f", s_smooth.get(0x05));
    lv_label_set_text_fmt(objects.label_ram_available, "%.1f", s_smooth.get(0x06));
    lv_label_set_text_fmt(objects.label_ram_virtual_used, "%.1f", s_smooth.get(0x08));
    lv_label_set_text_fmt(objects.label_ram_virtual_available, "%.1f", s_smooth.get(0x09));
    lv_label_set_text_fmt(objects.label_ram_percentage_details, "%.1f", s_smooth.get(0x07));
    lv_arc_set_value(objects.arc_ram_percentage_details, s_smooth.getInt(0x07));
    lv_label_set_text_fmt(objects.label_ram_percentage_virtual_details, "%.1f", s_smooth.get(0x0A));
    lv_arc_set_value(objects.arc_ram_percentage_virtual_details, s_smooth.getInt(0x0A));
}

void displayData()
{

    uint8_t active_index = lv_tabview_get_tab_active(objects.tabview);
    if (active_index == Tabs::ALL)
    {
    }
    else if (active_index == Tabs::CPU)
    {
        displayCpuDetails();
    }
    else if (active_index == Tabs::GPU)
    {
        displayGpuDetails();
    }
    else if (active_index == Tabs::RAM)
    {
        displayRamDetails();
    }
}

void updateSmoothTargets()
{
    s_smooth.setTarget(0x01, monitor.get(0x01));
    s_smooth.setTarget(0x02, monitor.get(0x02));
    s_smooth.setTarget(0x03, monitor.get(0x03));
    s_smooth.setTarget(0x04, monitor.get(0x04));
    s_smooth.setTarget(0x05, monitor.get(0x05));
    s_smooth.setTarget(0x06, monitor.get(0x06));
    s_smooth.setTarget(0x07, monitor.get(0x07));
    s_smooth.setTarget(0x08, monitor.get(0x08));
    s_smooth.setTarget(0x09, monitor.get(0x09));
    s_smooth.setTarget(0x0A, monitor.get(0x0A));
    s_smooth.setTarget(0x0B, monitor.get(0x0B));
    s_smooth.setTarget(0x0C, monitor.get(0x0C));
    s_smooth.setTarget(0x0D, monitor.get(0x0D));
    s_smooth.setTarget(0x0E, monitor.get(0x0E));
    s_smooth.setTarget(0x0F, monitor.get(0x0F));
    s_smooth.setTarget(0x10, monitor.get(0x10));
    s_smooth.setTarget(0x11, monitor.get(0x11));
}
float test = 0.0f;
void loop()
{

    if (monitor.update(Serial))
    {
        updateSmoothTargets();
    }

    s_smooth.updateAll();
    /* let the GUI do its work */
    static uint32_t lastTick = 0;
    uint32_t now = millis();
    if (now - lastTick >= 5)
    {
        lv_tick_inc(now - lastTick);
        lastTick = now;
    }

    ui_tick();
    displayData();
    lv_timer_handler();
    // test += 0.1f;
    // if (test > 100.0f)
    //     test = 0.0f;
    //      lv_label_set_text_fmt(objects.label_gpu_power, "%.1f", test);
    // taskManager.runLoop();
    lv_label_set_text_fmt(objects.test, "FPS: %.1f", s_smooth.get(0x04));
    // delay(1);
}
