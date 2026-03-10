#ifndef KEY_H
#define KEY_H

#include "stm32f1xx_hal_gpio.h"

#define KEY_PRESSED 0
#define KEY_RELEASED 1

typedef enum {
    STATE_IDLE = 0,    // 空闲状态
    STATE_DEBOUNCE,    // 防抖确认状态
    STATE_WAIT_RELEASE // 等待释放状态
} KeyState_t;

void key_scan_task(GPIO_TypeDef *Key_GPIO_Port, uint16_t Key_GPIO_Pin);

#endif /* KEY_H */