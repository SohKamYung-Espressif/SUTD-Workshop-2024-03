/* Get Start Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 0)
#include "esp_mac.h"
#endif

#include "espnow.h"
#include "espnow_storage.h"
#include "espnow_utils.h"

#include "app_priv.h"

static const char *TAG = "app_main";

static void app_wifi_init()
{
    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static esp_err_t espnow_data_recv_cb(uint8_t *src_addr, void *data,
                                       size_t size, wifi_pkt_rx_ctrl_t *rx_ctrl)
{
    ESP_PARAM_CHECK(src_addr);
    ESP_PARAM_CHECK(data);
    ESP_PARAM_CHECK(size);
    ESP_PARAM_CHECK(rx_ctrl);

    static uint32_t count = 0;

    ESP_LOGI(TAG, "espnow_recv, <%" PRIu32 "> [" MACSTR "][%d][%d][%u]: %.*s",
             count++, MAC2STR(src_addr), rx_ctrl->channel, rx_ctrl->rssi, size, size, (char *)data);

    if (strncmp(INITIATOR_NAME, (char *)data, strlen(INITIATOR_NAME)) == 0) {
        /* parse the {GPIO_PIN} at the end of the message */
        /* +1 means to skip the "/" before {GPIO_PIN} */
        int gpio_pin = atoi((char *)data + strlen(INITIATOR_NAME) + 1);
        ESP_LOGI(TAG, "Received toggle command on GPIO(%d)", gpio_pin);
        app_driver_gpio_toggle(gpio_pin);
    }

    return ESP_OK;
}

static void local_mac_addr_broadcast()
{
    static espnow_frame_head_t frame_head = {
        .retransmit_count = CONFIG_RETRY_NUM,
        .broadcast        = true,
    };

    while (true) {
        ESP_LOGI(TAG, "Broadcasting name %s to initiators", RESPONDER_NAME);
        espnow_send(ESPNOW_DATA_TYPE_DATA, ESPNOW_ADDR_BROADCAST, RESPONDER_NAME, strlen(RESPONDER_NAME)+1, &frame_head, portMAX_DELAY);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    espnow_storage_init();

    app_driver_gpio_init();
    app_driver_gpio_self_check();

    app_wifi_init();

    espnow_config_t espnow_config = ESPNOW_INIT_CONFIG_DEFAULT();

    espnow_init(&espnow_config);

    xTaskCreate(local_mac_addr_broadcast, "mac_broad", 2048, NULL, 4, NULL);

    espnow_set_config_for_data_type(ESPNOW_DATA_TYPE_DATA, true, espnow_data_recv_cb);
}
