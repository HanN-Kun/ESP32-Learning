#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define LED_PIN    GPIO_NUM_26
#define BUTTON_PIN GPIO_NUM_0   // kartın BOOT butonu

static QueueHandle_t buton_kuyrugu;

// Kesme fonksiyonu: çok kısa, sadece kuyruğa not bırakır
static void IRAM_ATTR buton_isr(void *arg)
{
    uint32_t pin = (uint32_t)arg;
    xQueueSendFromISR(buton_kuyrugu, &pin, NULL);
}

// LED görevi: kuyruktan not gelince LED'i değiştirir
static void led_gorevi(void *arg)
{
    uint32_t pin;
    int led_durum = 0;
    TickType_t son_zaman = 0;

    while (1) {
        // Not gelene kadar burada uyur
        if (xQueueReceive(buton_kuyrugu, &pin, portMAX_DELAY)) {
            TickType_t simdi = xTaskGetTickCount();

            // Son basıştan 200 ms geçtiyse gerçek basış say (debounce)
            if ((simdi - son_zaman) > pdMS_TO_TICKS(200)) {
                led_durum = !led_durum;
                gpio_set_level(LED_PIN, led_durum);
                printf("Buton kesmesi geldi, LED: %s\n", led_durum ? "YANIK" : "SONUK");
                son_zaman = simdi;
            }
        }
    }
}

void app_main(void)
{
    // LED pini: çıkış
    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_cfg);

    // Buton pini: giriş, basma anında (düşen kenar) kesme üretir
    gpio_config_t btn_cfg = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&btn_cfg);

    // Kuyruk ve LED görevi
    buton_kuyrugu = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(led_gorevi, "led_gorevi", 2048, NULL, 10, NULL);

    // Kesme servisini kur ve buton pinine kesme fonksiyonunu bağla
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, buton_isr, (void *)BUTTON_PIN);

    // Ana döngü: butondan bağımsız kendi işini yapar
    while (1) {
        printf("Ana döngü çalışıyor...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}