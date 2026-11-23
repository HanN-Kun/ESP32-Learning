#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN    GPIO_NUM_26
#define BUTTON_PIN GPIO_NUM_0   // kartın BOOT butonu

void app_main(void)
{
    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_cfg);

    gpio_config_t btn_cfg = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&btn_cfg);

    int led_durum = 0;
    gpio_set_level(LED_PIN, led_durum);

    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0) {     
            vTaskDelay(pdMS_TO_TICKS(50));         

            if (gpio_get_level(BUTTON_PIN) == 0) {  
                led_durum = !led_durum;           
                gpio_set_level(LED_PIN, led_durum);
                printf("LED: %s\n", led_durum ? "YANIK" : "SONUK");

                while (gpio_get_level(BUTTON_PIN) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(10));  
                }
                vTaskDelay(pdMS_TO_TICKS(50));      
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}