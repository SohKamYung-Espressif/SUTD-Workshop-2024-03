#include "app_priv.h"
#include "iot_button.h"

#define BUTTON_GPIO_PIN GPIO_NUM_9

static const char *TAG = "device";

static button_config_t button_driver_get_config()
{
    button_config_t config = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = BUTTON_GPIO_PIN,
            .active_level = 0,
        }
    };
    return config;
}

void app_driver_button_init(button_cb_t button_cb)
{
    button_config_t config = button_driver_get_config();
    button_handle_t handle = iot_button_create(&config);

    /* register handler to BUTTON_PRESS_DOWN event */
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, button_cb, NULL);
}
