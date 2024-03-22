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

static uint8_t RESPONDER_ADDR[ETH_ADDR_LEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

/** Converts a human readable Wi-Fi data rate in ESP-IDF enum value
 * Unknown rates will return the default 1 Mbits/s rate
 *
 * The Wi-Fi rates shown here are a subset of the supported rates.
 * For the full set of rates, check typedef struct wifi_phy_rate_t in ESP-IDF.
 * For ESP-IDF v5.2 the definition can be found at
 * [ https://github.com/espressif/esp-idf/blob/release/v5.2/components/esp_wifi/include/esp_wifi_types.h ]
 */

static wifi_phy_rate_t app_get_esp_wifi_rate(unsigned rate)
{
    switch (rate) {
        case 1:
            return WIFI_PHY_RATE_1M_L;
        case 2:
            return WIFI_PHY_RATE_2M_L;
        case 5:
            return WIFI_PHY_RATE_5M_L;
        case 6:
            return WIFI_PHY_RATE_6M;
        case 9:
            return WIFI_PHY_RATE_9M;
        case 11:
            return WIFI_PHY_RATE_11M_L;
        case 12:
            return WIFI_PHY_RATE_12M;
        case 18:
            return WIFI_PHY_RATE_18M;
        case 24:
            return WIFI_PHY_RATE_24M;
        case 36:
            return WIFI_PHY_RATE_36M;
        case 48:
            return WIFI_PHY_RATE_48M;
        case 54:
            return WIFI_PHY_RATE_54M;
        default:
            ESP_LOGW(TAG, "Unknown data rate. Setting to 1 Mbits/s");
            return WIFI_PHY_RATE_1M_L;
    }
}

static void app_wifi_init()
{
    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void button_toggle_cb(void *arg, void *priv_data)
{
    static espnow_frame_head_t frame_head = {
        .retransmit_count = CONFIG_RETRY_NUM,
        .broadcast        = false,
    };

    /* if not yet bound to responder, never send toggle command */
    if (memcmp(RESPONDER_ADDR, ESPNOW_ADDR_BROADCAST, ETH_ADDR_LEN) == 0) {
        ESP_LOGI(TAG, "Toggle command not sent. Still waiting to bind with responder");
    }

    else {
        /* create outbound data and send */
        char toggle_command[ESPNOW_DATA_LEN];
        memset(toggle_command, 0, ESPNOW_DATA_LEN);
        strcat(toggle_command, INITIATOR_NAME);
        strcat(toggle_command, "/");

        /* attach gpio to toggle command */
        strcat(toggle_command, CONTROL_PIN); /* $INITIATOR/GROUP_{X}/{GPIO_PIN} */
        ESP_LOGI(TAG, "Sending data: %s", toggle_command);
        espnow_send(ESPNOW_DATA_TYPE_DATA, RESPONDER_ADDR, toggle_command, strlen(toggle_command)+1, &frame_head, portMAX_DELAY);
    }
}

static esp_err_t espnow_data_recv_cb(uint8_t *src_addr, void *data,
                                       size_t size, wifi_pkt_rx_ctrl_t *rx_ctrl)
{
    wifi_phy_rate_t data_rate;

    ESP_PARAM_CHECK(src_addr);
    ESP_PARAM_CHECK(data);
    ESP_PARAM_CHECK(size);
    ESP_PARAM_CHECK(rx_ctrl);

    static uint32_t count = 0;

    /* only print received data before we bind responder */
    if (memcmp(RESPONDER_ADDR, ESPNOW_ADDR_BROADCAST, ETH_ADDR_LEN) == 0) {
        ESP_LOGI(TAG, "espnow_recv, <%" PRIu32 "> [" MACSTR "][%d][%d][%u]: %.*s",
                count++, MAC2STR(src_addr), rx_ctrl->channel, rx_ctrl->rssi, size, size, (char *)data);

        if (strncmp(RESPONDER_NAME, (char *)data, strlen(RESPONDER_NAME)) == 0) {
            ESP_LOGI(TAG, "Bind to RESPONDER: %s", (char *)data);
            memcpy(RESPONDER_ADDR, src_addr, ETH_ADDR_LEN);
            espnow_add_peer(src_addr, NULL);

            // set data rate
            data_rate = app_get_esp_wifi_rate(WIFI_DATA_RATE);
            esp_now_rate_config_t rate_config = {
                .phymode = WIFI_PHY_MODE_11B,
                .rate = data_rate,
                .ersu = 0,
                .dcm = 0,
            };
            ESP_ERROR_CHECK(esp_now_set_peer_rate_config(src_addr, &rate_config));
        }
    }

    return ESP_OK;
}

void app_main()
{
    espnow_storage_init();

    app_driver_button_init(button_toggle_cb);

    app_wifi_init();

    espnow_config_t espnow_config = ESPNOW_INIT_CONFIG_DEFAULT();

    espnow_init(&espnow_config);

    espnow_set_config_for_data_type(ESPNOW_DATA_TYPE_DATA, true, espnow_data_recv_cb);
}
