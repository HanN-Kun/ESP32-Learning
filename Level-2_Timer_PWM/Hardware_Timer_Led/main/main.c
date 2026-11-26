#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_attr.h"

#define LED_PIN      GPIO_NUM_26
#define PERIYOT_US   500000        // 500 ms

static volatile uint32_t kesme_sayisi = 0;
static bool led_durumu = false;

// Timer her hedefe ulaştığında çalışır (kesme içinde, kısa tut)
static bool IRAM_ATTR timer_kesmesi(gptimer_handle_t timer,
                                    const gptimer_alarm_event_data_t *veri,
                                    void *kullanici)
{
    led_durumu = !led_durumu;
    gpio_set_level(LED_PIN, led_durumu);
    kesme_sayisi++;
    return false;   // bir görev uyandırmadık
}

void app_main(void)
{
    // 1) LED pini
    gpio_config_t led_ayar = {
        .pin_bit_mask = 1ULL << LED_PIN,
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(gpio_config(&led_ayar));

    // 2) Timer: 1 MHz, yani 1 sayım = 1 mikrosaniye
    gptimer_handle_t timer = NULL;
    gptimer_config_t timer_ayar = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_ayar, &timer));

    // 3) Kesme fonksiyonunu bağla
    gptimer_event_callbacks_t olaylar = {
        .on_alarm = timer_kesmesi,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer, &olaylar, NULL));

    // 4) Alarm: hedefe gelince kesme üret, sayacı sıfırla
    gptimer_alarm_config_t alarm_ayar = {
        .alarm_count = PERIYOT_US,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(timer, &alarm_ayar));

    // 5) Başlat
    ESP_ERROR_CHECK(gptimer_enable(timer));
    ESP_ERROR_CHECK(gptimer_start(timer));

    while (1) {
        printf("Kesme sayisi: %" PRIu32 "\n", kesme_sayisi);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}