#include "app_key.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_attr.h"

#include "app_status.h"
#include "app_servo.h"

#define KEY_GPIO_PIN 10   // high active

#define KEY_TASK_PRIORITY 2
#define KEY_TASK_SIZE   2048

static QueueHandle_t key_queue = NULL;

void key_task(void *arg);

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(key_queue, &gpio_num, NULL);
}

void key_io_init(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL<<KEY_GPIO_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    //install gpio isr service
    gpio_install_isr_service(0);

    gpio_isr_handler_add(KEY_GPIO_PIN, gpio_isr_handler, (void*) KEY_GPIO_PIN);

    key_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(key_task, "keytesk", KEY_TASK_SIZE, NULL, KEY_TASK_PRIORITY, NULL);
}

void key_task(void *arg)
{   
    uint32_t key_num;
    uint8_t last_key_state = 0;

    while (1) {
        if (xQueueReceive(key_queue, &key_num, portMAX_DELAY)) {
            vTaskDelay(20 / portTICK_PERIOD_MS);  // Debouncing delay
            uint8_t current_key_state = gpio_get_level(KEY_GPIO_PIN);

            if (current_key_state != last_key_state) {
                last_key_state = current_key_state;
                if (current_key_state == 0) {
                    printf("key released\n");
                
                    app_status_set_switch_state(!get_switch_state());
                    servo_app_switch(get_switch_state());
                    
                } else {
                    printf("key pressed\n");
                }
            }
        }
    }
}