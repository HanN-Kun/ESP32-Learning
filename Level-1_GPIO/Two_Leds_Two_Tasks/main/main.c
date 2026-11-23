#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED1_PIN GPIO_NUM_26
#define LED2_PIN GPIO_NUM_27

// LED1 görevi: hızlı yanıp söner
static void led1_gorevi(void *arg)
{
    int durum = 0;
    while (1) {
        durum = !durum;
        gpio_set_level(LED1_PIN, durum);
        printf("LED1 -> %d\n", durum);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

// LED2 görevi: yavaş yanıp söner
static void led2_gorevi(void *arg)
{
    int durum = 0;
    while (1) {
        durum = !durum;
        gpio_set_level(LED2_PIN, durum);
        printf("LED2 -> %d\n", durum);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    // İki LED pinini birlikte çıkış olarak ayarla
    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << LED1_PIN) | (1ULL << LED2_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_cfg);

    // İki görevi başlat
    xTaskCreate(led1_gorevi, "led1_gorevi", 2048, NULL, 5, NULL);
    xTaskCreate(led2_gorevi, "led2_gorevi", 2048, NULL, 5, NULL);
}