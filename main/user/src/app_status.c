#include "app_status.h"

#include "esp_log.h"

#define TAG = "APP_STATUS"

app_status_t app_status;

void app_status_init(void)
{
    app_status.switch_state = false;
}

uint8_t get_switch_state(void)
{
    printf("switch state:%d\n", app_status.switch_state);
    return app_status.switch_state;
}

void app_status_set_switch_state(uint8_t switch_state)
{
    app_status.switch_state = switch_state;
}

