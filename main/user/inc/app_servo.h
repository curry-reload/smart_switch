#ifndef __APP_SERVO_H_
#define __APP_SERVO_H_


typedef enum {
    SWITCH_NO_INIT,
    SWITCH_OFF,
    SWITCH_ON,
}switc_state_e;

void servo_app_init(void);
void servo_test(void);

#endif
