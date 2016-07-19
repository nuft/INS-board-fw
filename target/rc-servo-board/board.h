/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F3-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F3_DISCOVERY
#define BOARD_NAME                  "RC Servo board"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#define STM32_LSEDRV                (3U << 3U)

#define STM32_HSECLK                16000000U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F303xC

/*
 * IO pins assignments.
 */
#define GPIOA_IN1                   0
#define GPIOA_IN2                   1
#define GPIOA_IN3                   2
#define GPIOA_IN4                   3 // UART2_RX
#define GPIOA_NC4                   4
#define GPIOA_NC5                   5
#define GPIOA_SERVO1                6
#define GPIOA_SERVO2                7
#define GPIOA_CAN_EN                8 // active low
#define GPIOA_UART1_TX              9
#define GPIOA_UART1_RX              10
#define GPIOA_CAN_RX                11
#define GPIOA_CAN_TX                12
#define GPIOA_TMS_SWDIO             13
#define GPIOA_TCK_SWCLK             14
#define GPIOA_TDI                   15

#define GPIOB_SERVO3                0
#define GPIOB_SERVO4                1
#define GPIOB_V_BAT_ADC             2
#define GPIOB_TDO                   3
#define GPIOB_TRST                  4
#define GPIOB_IN_PPM1               5
#define GPIOB_SERVO5                6
#define GPIOB_SERVO6                7
#define GPIOB_I2C_SCL               8
#define GPIOB_I2C_SDA               9
#define GPIOB_USART3_TX             10
#define GPIOB_USART3_RX             11
#define GPIOB_NC_12                 12
#define GPIOB_IN_PPM2               13
#define GPIOB_SERVO7                14
#define GPIOB_SERVO8                15

#define GPIOC_LED_HEARTBEAT         13
#define GPIOC_LED_STATUS            14
#define GPIOC_LED_ERROR             15

#define GPIOF_OSC_IN                0
#define GPIOF_OSC_OUT               1


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_10M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 *
 * PA0  - GPIOA_IN1                 (input pulldown)
 * PA1  - GPIOA_IN2                 (input pulldown)
 * PA2  - GPIOA_IN3                 (input pulldown)
 * PA3  - GPIOA_IN4                 (alternate 7, pullup) UART2_RX
 * PA4  - GPIOA_NC4                 (input pulldown)
 * PA5  - GPIOA_NC5                 (input pulldown)
 * PA6  - GPIOA_SERVO1              (alternate 2, TIM3_CH1)
 * PA7  - GPIOA_SERVO2              (alternate 2, TIM3_CH2)
 * PA8  - GPIOA_CAN_EN              (output high)
 * PA9  - GPIOA_UART1_TX            (alternate 7)
 * PA10 - GPIOA_UART1_RX            (alternate 7, pullup)
 * PA11 - GPIOA_CAN_RX              (alternate 9)
 * PA12 - GPIOA_CAN_TX              (alternate 9)
 * PA13 - GPIOA_TMS_SWDIO           (alternate 0, pullup)
 * PA14 - GPIOA_TCK_SWCLK           (alternate 0, pulldown)
 * PA15 - GPIOA_TDI                 (alternate 0, pulldown)
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_IN1) | \
                                     PIN_MODE_INPUT(GPIOA_IN2) | \
                                     PIN_MODE_INPUT(GPIOA_IN3) | \
                                     PIN_MODE_ALTERNATE(GPIOA_IN4) | \
                                     PIN_MODE_INPUT(GPIOA_NC4) | \
                                     PIN_MODE_INPUT(GPIOA_NC5) | \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO1) | \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO2) | \
                                     PIN_MODE_OUTPUT(GPIOA_CAN_EN) | \
                                     PIN_MODE_ALTERNATE(GPIOA_UART1_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOA_UART1_RX) | \
                                     PIN_MODE_ALTERNATE(GPIOA_CAN_RX) | \
                                     PIN_MODE_ALTERNATE(GPIOA_CAN_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOA_TMS_SWDIO) | \
                                     PIN_MODE_ALTERNATE(GPIOA_TCK_SWCLK) | \
                                     PIN_MODE_ALTERNATE(GPIOA_TDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_IN1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_IN2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_IN3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_IN4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_NC4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_NC5) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CAN_EN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART1_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART1_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CAN_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CAN_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TMS_SWDIO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TCK_SWCLK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_50M(GPIOA_IN1) | \
                                     PIN_OSPEED_50M(GPIOA_IN2) | \
                                     PIN_OSPEED_50M(GPIOA_IN3) | \
                                     PIN_OSPEED_50M(GPIOA_IN4) | \
                                     PIN_OSPEED_50M(GPIOA_NC4) | \
                                     PIN_OSPEED_50M(GPIOA_NC5) | \
                                     PIN_OSPEED_50M(GPIOA_SERVO1) | \
                                     PIN_OSPEED_50M(GPIOA_SERVO2) | \
                                     PIN_OSPEED_50M(GPIOA_CAN_EN) | \
                                     PIN_OSPEED_50M(GPIOA_UART1_TX) | \
                                     PIN_OSPEED_50M(GPIOA_UART1_RX) | \
                                     PIN_OSPEED_50M(GPIOA_CAN_RX) | \
                                     PIN_OSPEED_50M(GPIOA_CAN_TX) | \
                                     PIN_OSPEED_50M(GPIOA_TMS_SWDIO) | \
                                     PIN_OSPEED_50M(GPIOA_TCK_SWCLK) | \
                                     PIN_OSPEED_50M(GPIOA_TDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOA_IN1) | \
                                     PIN_PUPDR_PULLDOWN(GPIOA_IN2) | \
                                     PIN_PUPDR_PULLDOWN(GPIOA_IN3) | \
                                     PIN_PUPDR_PULLUP(GPIOA_IN4) | \
                                     PIN_PUPDR_PULLDOWN(GPIOA_NC4) | \
                                     PIN_PUPDR_PULLDOWN(GPIOA_NC5) | \
                                     PIN_PUPDR_FLOATING(GPIOA_SERVO1) | \
                                     PIN_PUPDR_FLOATING(GPIOA_SERVO2) | \
                                     PIN_PUPDR_FLOATING(GPIOA_CAN_EN) | \
                                     PIN_PUPDR_FLOATING(GPIOA_UART1_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOA_UART1_RX) | \
                                     PIN_PUPDR_FLOATING(GPIOA_CAN_RX) | \
                                     PIN_PUPDR_FLOATING(GPIOA_CAN_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOA_TMS_SWDIO) | \
                                     PIN_PUPDR_PULLDOWN(GPIOA_TCK_SWCLK) | \
                                     PIN_PUPDR_PULLDOWN(GPIOA_TDI))
#define VAL_GPIOA_ODR               (PIN_ODR_LOW(GPIOA_IN1) | \
                                     PIN_ODR_LOW(GPIOA_IN2) | \
                                     PIN_ODR_LOW(GPIOA_IN3) | \
                                     PIN_ODR_LOW(GPIOA_IN4) | \
                                     PIN_ODR_LOW(GPIOA_NC4) | \
                                     PIN_ODR_LOW(GPIOA_NC5) | \
                                     PIN_ODR_LOW(GPIOA_SERVO1) | \
                                     PIN_ODR_LOW(GPIOA_SERVO2) | \
                                     PIN_ODR_HIGH(GPIOA_CAN_EN) | \
                                     PIN_ODR_LOW(GPIOA_UART1_TX) | \
                                     PIN_ODR_LOW(GPIOA_UART1_RX) | \
                                     PIN_ODR_LOW(GPIOA_CAN_RX) | \
                                     PIN_ODR_LOW(GPIOA_CAN_TX) | \
                                     PIN_ODR_LOW(GPIOA_TMS_SWDIO) | \
                                     PIN_ODR_LOW(GPIOA_TCK_SWCLK) | \
                                     PIN_ODR_LOW(GPIOA_TDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_IN1, 0) | \
                                     PIN_AFIO_AF(GPIOA_IN2, 0) | \
                                     PIN_AFIO_AF(GPIOA_IN3, 0) | \
                                     PIN_AFIO_AF(GPIOA_IN4, 7) | \
                                     PIN_AFIO_AF(GPIOA_NC4, 0) | \
                                     PIN_AFIO_AF(GPIOA_NC5, 0) | \
                                     PIN_AFIO_AF(GPIOA_SERVO1, 2) | \
                                     PIN_AFIO_AF(GPIOA_SERVO2, 2))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_CAN_EN, 0) | \
                                     PIN_AFIO_AF(GPIOA_UART1_TX, 7) | \
                                     PIN_AFIO_AF(GPIOA_UART1_RX, 7) | \
                                     PIN_AFIO_AF(GPIOA_CAN_RX, 9) | \
                                     PIN_AFIO_AF(GPIOA_CAN_TX, 9) | \
                                     PIN_AFIO_AF(GPIOA_TMS_SWDIO, 0) | \
                                     PIN_AFIO_AF(GPIOA_TCK_SWCLK, 0) | \
                                     PIN_AFIO_AF(GPIOA_TDI, 0))

/*
 * GPIOB setup:
 *
 * PB0  - GPIOB_SERVO3              (alternate 2, TIM3_CH3)
 * PB1  - GPIOB_SERVO4              (alternate 2, TIM3_CH4)
 * PB2  - GPIOB_V_BAT_ADC           (analog)
 * PB3  - GPIOB_TDO                 (alternate 0, pulldown)
 * PB4  - GPIOB_TRST                (alternate 0, floating)
 * PB5  - GPIOB_IN_PPM1             (input pulldown)
 * PB6  - GPIOB_SERVO5              (alternate 2, TIM4_CH1)
 * PB7  - GPIOB_SERVO6              (alternate 2, TIM4_CH2)
 * PB8  - GPIOB_I2C_SCL             (alternate 4, I2C1)
 * PB9  - GPIOB_I2C_SDA             (alternate 4, I2C1)
 * PB10 - GPIOB_USART3_TX           (alternate 7, USART3_TX)
 * PB11 - GPIOB_USART3_RX           (alternate 7, USART3_RX, pullup)
 * PB12 - GPIOB_NC_12               (input pulldown)
 * PB13 - GPIOB_IN_PPM2             (input pulldown)
 * PB14 - GPIOB_SERVO7              (alternate 1, TIM15_CH1)
 * PB15 - GPIOB_SERVO8              (alternate 1, TIM15_CH2)
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ALTERNATE(GPIOB_SERVO3) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO4) | \
                                     PIN_MODE_ANALOG(GPIOB_V_BAT_ADC) | \
                                     PIN_MODE_ALTERNATE(GPIOB_TDO) | \
                                     PIN_MODE_ALTERNATE(GPIOB_TRST) | \
                                     PIN_MODE_INPUT(GPIOB_IN_PPM1) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO5) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO6) | \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C_SCL) | \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C_SDA) | \
                                     PIN_MODE_ALTERNATE(GPIOB_USART3_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOB_USART3_RX) | \
                                     PIN_MODE_INPUT(GPIOB_NC_12) | \
                                     PIN_MODE_INPUT(GPIOB_IN_PPM2) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO7) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO8))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_SERVO3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_V_BAT_ADC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_TDO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_TRST) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_IN_PPM1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO5) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO6) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C_SCL) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C_SDA) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USART3_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USART3_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_NC_12) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_IN_PPM2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO7) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO8))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_50M(GPIOB_SERVO3) | \
                                     PIN_OSPEED_50M(GPIOB_SERVO4) | \
                                     PIN_OSPEED_50M(GPIOB_V_BAT_ADC) | \
                                     PIN_OSPEED_50M(GPIOB_TDO) | \
                                     PIN_OSPEED_50M(GPIOB_TRST) | \
                                     PIN_OSPEED_50M(GPIOB_IN_PPM1) | \
                                     PIN_OSPEED_50M(GPIOB_SERVO5) | \
                                     PIN_OSPEED_50M(GPIOB_SERVO6) | \
                                     PIN_OSPEED_50M(GPIOB_I2C_SCL) | \
                                     PIN_OSPEED_50M(GPIOB_I2C_SDA) | \
                                     PIN_OSPEED_50M(GPIOB_USART3_TX) | \
                                     PIN_OSPEED_50M(GPIOB_USART3_RX) | \
                                     PIN_OSPEED_50M(GPIOB_NC_12) | \
                                     PIN_OSPEED_50M(GPIOB_IN_PPM2) | \
                                     PIN_OSPEED_50M(GPIOB_SERVO7) | \
                                     PIN_OSPEED_50M(GPIOB_SERVO8))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_SERVO3) | \
                                     PIN_PUPDR_FLOATING(GPIOB_SERVO4) | \
                                     PIN_PUPDR_FLOATING(GPIOB_V_BAT_ADC) | \
                                     PIN_PUPDR_PULLDOWN(GPIOB_TDO) | \
                                     PIN_PUPDR_FLOATING(GPIOB_TRST) | \
                                     PIN_PUPDR_PULLDOWN(GPIOB_IN_PPM1) | \
                                     PIN_PUPDR_FLOATING(GPIOB_SERVO5) | \
                                     PIN_PUPDR_FLOATING(GPIOB_SERVO6) | \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C_SCL) | \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C_SDA) | \
                                     PIN_PUPDR_FLOATING(GPIOB_USART3_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOB_USART3_RX) | \
                                     PIN_PUPDR_PULLDOWN(GPIOB_NC_12) | \
                                     PIN_PUPDR_PULLDOWN(GPIOB_IN_PPM2) | \
                                     PIN_PUPDR_FLOATING(GPIOB_SERVO7) | \
                                     PIN_PUPDR_FLOATING(GPIOB_SERVO8))
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_SERVO3) | \
                                     PIN_ODR_LOW(GPIOB_SERVO4) | \
                                     PIN_ODR_LOW(GPIOB_V_BAT_ADC) | \
                                     PIN_ODR_LOW(GPIOB_TDO) | \
                                     PIN_ODR_LOW(GPIOB_TRST) | \
                                     PIN_ODR_LOW(GPIOB_IN_PPM1) | \
                                     PIN_ODR_LOW(GPIOB_SERVO5) | \
                                     PIN_ODR_LOW(GPIOB_SERVO6) | \
                                     PIN_ODR_LOW(GPIOB_I2C_SCL) | \
                                     PIN_ODR_LOW(GPIOB_I2C_SDA) | \
                                     PIN_ODR_LOW(GPIOB_USART3_TX) | \
                                     PIN_ODR_LOW(GPIOB_USART3_RX) | \
                                     PIN_ODR_LOW(GPIOB_NC_12) | \
                                     PIN_ODR_LOW(GPIOB_IN_PPM2) | \
                                     PIN_ODR_LOW(GPIOB_SERVO7) | \
                                     PIN_ODR_LOW(GPIOB_SERVO8))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_SERVO3, 2) | \
                                     PIN_AFIO_AF(GPIOB_SERVO4, 2) | \
                                     PIN_AFIO_AF(GPIOB_V_BAT_ADC, 0) | \
                                     PIN_AFIO_AF(GPIOB_TDO, 0) | \
                                     PIN_AFIO_AF(GPIOB_TRST, 0) | \
                                     PIN_AFIO_AF(GPIOB_IN_PPM1, 0) | \
                                     PIN_AFIO_AF(GPIOB_SERVO5, 2) | \
                                     PIN_AFIO_AF(GPIOB_SERVO6, 2))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C_SCL, 4) | \
                                     PIN_AFIO_AF(GPIOB_I2C_SDA, 4) | \
                                     PIN_AFIO_AF(GPIOB_USART3_TX, 7) | \
                                     PIN_AFIO_AF(GPIOB_USART3_RX, 7) | \
                                     PIN_AFIO_AF(GPIOB_NC_12, 0) | \
                                     PIN_AFIO_AF(GPIOB_IN_PPM2, 0) | \
                                     PIN_AFIO_AF(GPIOB_SERVO7, 1) | \
                                     PIN_AFIO_AF(GPIOB_SERVO8, 1))

/*
 * GPIOC setup:
 *
 * PC13 - GPIOC_LED_HEARTBEAT       (output low)
 * PC14 - GPIOC_LED_STATUS          (output low)
 * PC15 - GPIOC_LED_ERROR           (output low)
 */
#define VAL_GPIOC_MODER             (PIN_MODE_OUTPUT(GPIOC_LED_HEARTBEAT) | \
                                     PIN_MODE_OUTPUT(GPIOC_LED_STATUS) | \
                                     PIN_MODE_OUTPUT(GPIOC_LED_ERROR))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_LED_HEARTBEAT) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED_STATUS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED_ERROR))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_50M(GPIOC_LED_HEARTBEAT) | \
                                     PIN_OSPEED_50M(GPIOC_LED_STATUS) | \
                                     PIN_OSPEED_50M(GPIOC_LED_ERROR))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOC_LED_HEARTBEAT) | \
                                     PIN_PUPDR_PULLDOWN(GPIOC_LED_STATUS) | \
                                     PIN_PUPDR_PULLDOWN(GPIOC_LED_ERROR))
#define VAL_GPIOC_ODR               (PIN_ODR_LOW(GPIOC_LED_HEARTBEAT) | \
                                     PIN_ODR_LOW(GPIOC_LED_STATUS) | \
                                     PIN_ODR_LOW(GPIOC_LED_ERROR))
#define VAL_GPIOC_AFRL              (0)
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_LED_HEARTBEAT, 0) | \
                                     PIN_AFIO_AF(GPIOC_LED_STATUS, 0) | \
                                     PIN_AFIO_AF(GPIOC_LED_ERROR, 0))

/*
 * GPIOD setup:
 */
#define VAL_GPIOD_MODER             (0)
#define VAL_GPIOD_OTYPER            (0)
#define VAL_GPIOD_OSPEEDR           (0)
#define VAL_GPIOD_PUPDR             (0)
#define VAL_GPIOD_ODR               (0)
#define VAL_GPIOD_AFRL              (0)
#define VAL_GPIOD_AFRH              (0)

/*
 * GPIOE setup:
 */
#define VAL_GPIOE_MODER             (0)
#define VAL_GPIOE_OTYPER            (0)
#define VAL_GPIOE_OSPEEDR           (0)
#define VAL_GPIOE_PUPDR             (0)
#define VAL_GPIOE_ODR               (0)
#define VAL_GPIOE_AFRL              (0)
#define VAL_GPIOE_AFRH              (0)

/*
 * GPIOF setup:
 *
 * PF0  - OSC_IN                    (input floating)
 * PF1  - OSC_OUT                   (input floating)
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_OSC_IN) | \
                                     PIN_MODE_INPUT(GPIOF_OSC_OUT))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_OSC_IN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_OSC_OUT))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_50M(GPIOF_OSC_IN) | \
                                     PIN_OSPEED_50M(GPIOF_OSC_OUT))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_OSC_IN) | \
                                     PIN_PUPDR_FLOATING(GPIOF_OSC_OUT))
#define VAL_GPIOF_ODR               (PIN_ODR_LOW(GPIOF_OSC_IN) | \
                                     PIN_ODR_LOW(GPIOF_OSC_OUT))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_OSC_IN, 0) | \
                                     PIN_AFIO_AF(GPIOF_OSC_OUT, 0))
#define VAL_GPIOF_AFRH              (0)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
