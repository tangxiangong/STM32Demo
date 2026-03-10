#include "key.h"
#include <stdint.h>

void key_scan_task(GPIO_TypeDef *Key_GPIO_Port, uint16_t Key_GPIO_Pin) {
    static KeyState_t key_state = STATE_IDLE;
    GPIO_PinState current_level = HAL_GPIO_ReadPin(Key_GPIO_Port, Key_GPIO_Pin);

    switch (key_state) {
    case STATE_IDLE:
        if (current_level == KEY_PRESSED) {
            key_state = STATE_DEBOUNCE;
        }
        break;
    case STATE_DEBOUNCE:
        if (current_level == KEY_PRESSED) {
            // 触发按键事件
            key_state = STATE_WAIT_RELEASE;
        } else {
            key_state = STATE_IDLE;
        }
        break;
    case STATE_WAIT_RELEASE:
        if (current_level == KEY_RELEASED) {
            key_state = STATE_IDLE;
        }
        break;
    }
}

/**
 * #include "main.h"
 *
 * // 声明硬件初始化函数
 * void SystemClock_Config(void);
 * static void MX_GPIO_Init(void);
 *
 * // 1. 定义按键状态机的枚举
 * typedef enum {
 *     KEY_STATE_IDLE = 0,       // 空闲状态
 *     KEY_STATE_DEBOUNCE,       // 按下消抖状态
 *     KEY_STATE_WAIT_RELEASE    // 等待释放状态
 * } KeyState_t;
 *
 * int main(void)
 * {
 *     HAL_Init();
 *     SystemClock_Config();
 *     MX_GPIO_Init();
 *
 *     // 2. 状态机相关变量
 *     KeyState_t key_state = KEY_STATE_IDLE; // 初始状态为空闲
 *     uint32_t debounce_start_time = 0;      // 记录进入消抖状态的时间
 *     const uint32_t DEBOUNCE_TIME = 20;     // 消抖时间 20ms
 *
 *     while (1)
 *     {
 *         // 读取当前按键引脚电平 (PA0, 按下为低电平 RESET, 松开为高电平 SET)
 *         GPIO_PinState current_level = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
 *
 *         // 3. 执行按键状态机
 *         switch (key_state) {
 *
 *             case KEY_STATE_IDLE:
 *                 // 如果在空闲状态检测到低电平（按下）
 *                 if (current_level == GPIO_PIN_RESET) {
 *                     debounce_start_time = HAL_GetTick(); // 记录当前时间
 *                     key_state = KEY_STATE_DEBOUNCE;      // 切换到消抖状态
 *                 }
 *                 break;
 *
 *             case KEY_STATE_DEBOUNCE:
 *                 // 判断是否已经过了 20ms
 *                 if ((HAL_GetTick() - debounce_start_time) >= DEBOUNCE_TIME) {
 *                     // 20ms后再次确认电平
 *                     if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ==
 *IO_PIN_RESET) {
 *                         // 确实按下了，执行动作：翻转 LED (PC13)
 *                         HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
 *
 *                         // 动作执行完毕，切换到等待释放状态，防止一直触发
 *                         key_state = KEY_STATE_WAIT_RELEASE;
 *                     } else {
 *                         // 如果20ms后是高电平，说明是干扰或抖动，退回空闲状态
 *                         key_state = KEY_STATE_IDLE;
 *                     }
 *                 }
 *                 break
 *             case KEY_STATE_WAIT_RELEASE:
 *                 // 在等待释放状态，检测到高电平（松手）
 *                 if (current_level == GPIO_PIN_SET) {
 *                     //
 * （严格来说这里松手也会有抖动，通常简单应用直接切回空闲即可，
 *                     //  如果松手抖动严重，也可以在这里再加一个松手消抖状态）
 *                     key_state = KEY_STATE_IDLE; //到空闲状态，等待下一次按下
 *                 }
 *                 break
 *             default:
 *                 key_state = KEY_STATE_IDLE;
 *                 break;
 *         }
 *
 *         /
 *            这里是主循环的其他代码。
 *            由于状态机使用了 HAL_GetTick() 非阻塞延时，
 *            按键消抖的 20ms 期间，CPU 依然可以飞速运行到这里执行其他任务。
 *         /
 * }
 * }
 */