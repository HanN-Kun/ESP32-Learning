#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define LED_PIN       26
#define PWM_FREKANS   5000      // Hz
#define DUTY_MAX      1023      // 10 bit çözünürlük
#define DUTY_ADIM     10


// PWM timer'ını ve kanalını kurar
static void pwm_kur(void)
{
    // Timer: frekans ve çözünürlük
    ledc_timer_config_t timer_ayar =
    {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = PWM_FREKANS,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_ayar));

    // Kanal: hangi pin, hangi timer
    ledc_channel_config_t kanal_ayar =
    {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .gpio_num = LED_PIN,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&kanal_ayar));
}


// Parlaklığı ayarlar (0 - 1023)
static void parlaklik_ayarla(int duty)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}


void app_main(void)
{
    pwm_kur();

    while (1)
    {
        // Yavaşça parlat
        for (int duty = 0; duty <= DUTY_MAX; duty += DUTY_ADIM)
        {
            parlaklik_ayarla(duty);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Yavaşça söndür
        for (int duty = DUTY_MAX; duty >= 0; duty -= DUTY_ADIM)
        {
            parlaklik_ayarla(duty);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}