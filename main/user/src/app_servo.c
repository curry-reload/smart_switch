#include "app_servo.h"

#include "driver/ledc.h"
#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define SERVO_MIN_PULSEWIDTH_US (1000) // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US (2000) // Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE        (90)   // Maximum angle in degree upto which servo can rotate

#define SERVO_LEDC_TIMER              LEDC_TIMER_0
#define SERVO_LEDC_MODE               LEDC_LOW_SPEED_MODE
#define SERVO_IO                      (5) 
#define SERVO_LEDC_CHANNEL            LEDC_CHANNEL_0
#define SERVO_LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define SERVO_LEDC_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define SERVO_LEDC_FREQUENCY          (50) // Frequency in Hertz. Set frequency at 50Hz

#define SWITCH_ON_DEGREE        (10)
#define SWITCH_OFF_DEGREE       (-30)

void servo_driver_init(void);
static uint32_t _convert_servo_angle_to_duty(int angle);
static void servo_angle_ctrl(int angle);

/****** public function ******/
void servo_app_init(void)
{
    servo_driver_init();
}

void servo_app_switch(uint8_t onoff)
{   
    if (onoff == 1) {
        servo_angle_ctrl(SWITCH_ON_DEGREE);
    } else if (onoff == 0) {
        servo_angle_ctrl(SWITCH_OFF_DEGREE);
    }
}

void servo_test(void)
{    
    for(int i=(0-SERVO_MAX_DEGREE) ; i<=SERVO_MAX_DEGREE;i++){
        servo_angle_ctrl(i);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
    for(int i=SERVO_MAX_DEGREE; i>=(0-SERVO_MAX_DEGREE);i--){
        servo_angle_ctrl(i);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

/****** private function ******/
void servo_driver_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = SERVO_LEDC_MODE,
        .duty_resolution  = SERVO_LEDC_DUTY_RES,
        .timer_num        = SERVO_LEDC_TIMER,
        .freq_hz          = SERVO_LEDC_FREQUENCY,  
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = SERVO_LEDC_MODE,
        .channel        = SERVO_LEDC_CHANNEL,
        .timer_sel      = SERVO_LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = SERVO_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}


static uint32_t _convert_servo_angle_to_duty(int angle)
{
    uint32_t us;
    if((angle > SERVO_MAX_DEGREE) || (angle + SERVO_MAX_DEGREE < 0)){
        angle = 0;
    }
    us = SERVO_MIN_PULSEWIDTH_US + (angle + SERVO_MAX_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (2 * SERVO_MAX_DEGREE);
    //printf("angle: %d  us:%d\r\n",angle,us);
    return (int)(SERVO_LEDC_FREQUENCY * us * (1 << SERVO_LEDC_DUTY_RES) / 1000000);
}

static void servo_angle_ctrl(int angle)
{
    int duty = 0;

    duty = _convert_servo_angle_to_duty(angle);
    printf("Servo is on %d degree, %d duty\r\n", angle, duty);

    ledc_set_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL, duty);
    ledc_update_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL);
}