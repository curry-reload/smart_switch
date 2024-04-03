#include <stdio.h>

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_servo.h"

uint8_t switch_state = 0;

void app_main(void)
{
    printf("enter app_main\n");
    servo_app_init();

    while (1) {
        switch_state = !switch_state;
        servo_app_switch(switch_state);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
