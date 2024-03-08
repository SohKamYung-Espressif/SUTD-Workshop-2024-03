#ifndef APP_PRIV_H
#define APP_PRIV_H

#include "iot_button.h"
#include "esp_eth_spec.h"

#define RESPONDER_PREFIX                CONFIG_RESPONDER_PREFIX
#define INITIATOR_PREFIX                CONFIG_INITIATOR_PREFIX
#define GROUP_NAME                      CONFIG_GROUP_NAME

#define RESPONDER_NAME RESPONDER_PREFIX GROUP_NAME
#define INITIATOR_NAME INITIATOR_PREFIX GROUP_NAME
#define CONTROL_PIN    CONFIG_CONTROL_PIN

void app_driver_button_init(button_cb_t button_cb);
#endif /* APP_PRIV_H */