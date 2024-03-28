#include <stdio.h>

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_servo.h"

void app_main(void)
{
    printf("enter app_main\n");
    servo_app_init();

    while (1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
