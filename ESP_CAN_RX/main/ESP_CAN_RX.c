#include <stdio.h>
#include "esp_twai.h"
#include "esp_twai_onchip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static volatile bool rx_ready = false;
static volatile bool rx_callback_fired = false;
static volatile uint8_t rx_data = 0;
static volatile uint32_t rx_id = 0;

static bool twai_rx_cb(twai_node_handle_t handle,
                       const twai_rx_done_event_data_t *edata,
                       void *user_ctx)
{
    rx_callback_fired = true;

    uint8_t recv_buff[8];

    twai_frame_t rx_frame = {
        .buffer = recv_buff,
        .buffer_len = sizeof(recv_buff),
    };

    if (twai_node_receive_from_isr(handle, &rx_frame) == ESP_OK)
    {
        rx_id = rx_frame.header.id;

        if (rx_frame.header.dlc > 0)
        {
            rx_data = recv_buff[0];
            rx_ready = true;
        }
    }

    return false;
}

void app_main(void)
{
    twai_node_handle_t node_hdl = NULL;

    twai_onchip_node_config_t node_config = {
        .io_cfg.tx = 4,
        .io_cfg.rx = 5,
        .bit_timing = {
            .bitrate = 500000,
            .sp_permill = 875,
        },
        .tx_queue_depth = 5,
    };

    ESP_ERROR_CHECK(
        twai_new_node_onchip(&node_config, &node_hdl)
    );

    twai_mask_filter_config_t filter = {
        .id = 0,
        .mask = 0,
        .is_ext = false,
    };

    ESP_ERROR_CHECK(
        twai_node_config_mask_filter(node_hdl, 0, &filter)
    );

    twai_event_callbacks_t user_cbs = {
        .on_rx_done = twai_rx_cb,
    };

    ESP_ERROR_CHECK(
        twai_node_register_event_callbacks(node_hdl, &user_cbs, NULL)
    );

    ESP_ERROR_CHECK(
        twai_node_enable(node_hdl)
    );

    printf("TWAI receiver started\n");

    while (1)
    {
        if (rx_ready)
        {
            printf("CAN ID: 0x%03lX\n", (unsigned long)rx_id);
            printf("Data: 0x%02X\n", rx_data);

            rx_ready = false;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}