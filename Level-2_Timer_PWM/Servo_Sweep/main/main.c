#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define SERVO_PIN     13
#define PERIYOT_US    20000     // 50 Hz
#define DUTY_TOPLAM   16384     // 14 bit
#define DARBE_MIN_US  1000
#define DARBE_MAX_US  2000
#define ADIM_US       10


// Servo için PWM'i kurar
static void servo_kur(void)
{
    ledc_timer_config_t timer_ayar =
    {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_ayar));

    ledc_channel_config_t kanal_ayar =
    {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .gpio_num = SERVO_PIN,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&kanal_ayar));
}


// Darbe süresini (µs) duty değerine çevirip uygular
static void servo_darbe_ayarla(int darbe_us)
{
    int duty = (darbe_us * DUTY_TOPLAM) / PERIYOT_US;

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}


void app_main(void)
{
    servo_kur();

    while (1)
    {
        // Bir yöne süpür
        for (int us = DARBE_MIN_US; us <= DARBE_MAX_US; us += ADIM_US)
        {
            servo_darbe_ayarla(us);
            vTaskDelay(pdMS_TO_TICKS(20));
        }

        // Geri dön
        for (int us = DARBE_MAX_US; us >= DARBE_MIN_US; us -= ADIM_US)
        {
            servo_darbe_ayarla(us);
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}