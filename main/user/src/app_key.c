#include "app_key.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_attr.h"

#define KEY_GPIO_PIN 10


static void IRAM_ATTR gpio_isr_handler(void* arg)
{

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
}