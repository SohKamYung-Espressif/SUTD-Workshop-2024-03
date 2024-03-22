#ifndef APP_PRIV_H
#define APP_PRIV_H

#include <sdkconfig.h>
#include <esp_err.h>
#include <esp_types.h>
#include <hal/gpio_types.h>
#include "esp_eth_spec.h"
#include "hal/gpio_types.h"

#define RESPONDER_PREFIX                CONFIG_RESPONDER_PREFIX
#define INITIATOR_PREFIX                CONFIG_INITIATOR_PREFIX
#define GROUP_NAME                      CONFIG_GROUP_NAME

#define RESPONDER_NAME RESPONDER_PREFIX GROUP_NAME
#define INITIATOR_NAME INITIATOR_PREFIX GROUP_NAME

#if CONFIG_ENABLE_RSSI_FILTERING
#define ENABLE_RSSI_FILTERING           (1)
#define RSSI_FILTER_VALUE               (CONFIG_RSSI_FILTER_VALUE)
#else
#define ENABLE_RSSI_FILTERING           (0)
#endif

#define EXAMPLE_GPIO_MIN 0
#define EXAMPLE_GPIO_MAX 7
#define EXAMPLE_GPIO_NUM ( EXAMPLE_GPIO_MAX - EXAMPLE_GPIO_MIN + 1 )

void app_driver_gpio_init();
void app_driver_gpio_toggle(int pin_num);
void app_driver_gpio_self_check();

#endif /* APP_PRIV_H */