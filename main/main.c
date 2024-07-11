#include <stdio.h>

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_status.h"
#include "app_servo.h"
#include "app_key.h"

static void user_device_init(void)
{
    servo_app_init();
    key_io_init();
}

void app_main(void)
{
    printf("enter app_main\n");
    app_status_init();
    user_device_init();

    while (1) {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
