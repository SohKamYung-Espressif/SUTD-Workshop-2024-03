#include <app_priv.h>
#include <hal/gpio_ll.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "app_driver"

void app_driver_gpio_init()
{
    uint64_t pin_mask = 0;

    for (int pin_num=EXAMPLE_GPIO_MIN; pin_num<=EXAMPLE_GPIO_MAX; pin_num++) {
        pin_mask |= 1 << pin_num;
    }

    ESP_LOGI(TAG, "pin_mask=%08x", (uint32_t)pin_mask);

    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = pin_mask;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

void app_driver_gpio_toggle(int pin_num)
{
    if (pin_num < EXAMPLE_GPIO_MIN || pin_num > EXAMPLE_GPIO_MAX) {
        ESP_LOGE(TAG, "GPIO(%d) is out of range %d..%d", pin_num, EXAMPLE_GPIO_MIN, EXAMPLE_GPIO_MAX);
        return;
    }

    static bool gpio_level[EXAMPLE_GPIO_NUM] = {true, true, true, true, true, true, true, true};
    gpio_level[pin_num] = !gpio_level[pin_num];
    gpio_ll_set_level(&GPIO, pin_num, (int)gpio_level[pin_num]);

    ESP_LOGI(TAG, "set GPIO(%d) to %s", pin_num, gpio_level[pin_num]?"HIGH":"LOW");
}

/* bootup check */
void app_driver_gpio_self_check()
{
    for (int pin_num=EXAMPLE_GPIO_MIN; pin_num<=EXAMPLE_GPIO_MAX; pin_num++) {
        gpio_ll_set_level(&GPIO, pin_num, 1);
    }

    for (int pin_num=EXAMPLE_GPIO_MIN; pin_num<=EXAMPLE_GPIO_MAX; pin_num++) {
        gpio_ll_set_level(&GPIO, pin_num, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    for (int pin_num=EXAMPLE_GPIO_MIN; pin_num<=EXAMPLE_GPIO_MAX; pin_num++) {
        gpio_ll_set_level(&GPIO, pin_num, 1);
    }
}