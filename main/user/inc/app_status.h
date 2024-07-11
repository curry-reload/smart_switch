#ifndef __APP_STATUS_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    uint8_t switch_state;
}app_status_t;
extern app_status_t app_status;

void app_status_init(void);
uint8_t get_switch_state(void);
void app_status_set_switch_state(uint8_t switch_state);

#endif
