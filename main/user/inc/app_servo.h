#ifndef __APP_SERVO_H_
#define __APP_SERVO_H_

#include <stdio.h>

typedef enum {
    SWITCH_NO_INIT,
    SWITCH_OFF,
    SWITCH_ON,
}switc_state_e;

void servo_app_init(void);
/**
 * @brief servo control switch 
*/
void servo_app_switch(uint8_t onoff);
void servo_test(void);

#endif
