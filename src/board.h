#ifndef _BOARD_H_
#define _BOARD_H_

#define BOARD_NANOCOPTER
#define BOARD_NAME                  "Spieler Brothers - Nanocopter"


#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0
#endif

#define STM32_HSECLK                16000000

#define STM32_VDD                   330

#define STM32F405xx


#define UART_CONN1 SD1
#define UART_CONN2 SD4
#define UART_CONN3 SD2
#define UART_CONN4 SD6

/*
 * IO pins assignments.
 */
#define GPIOA_PB_CTRL_INT           0
#define GPIOA_BAT_NTC               1 // high = chg, low = off
#define GPIOA_CHG_STATUS            2 // low = chging
#define GPIOA_CHG_ISET              3 // high = 500mA max, low = ISET, float = 100mA max
#define GPIOA_NC_4                  4
#define GPIOA_NC_5                  5
#define GPIOA_NC_6                  6
#define GPIOA_NC_7                  7
#define GPIOA_MCO1_CLK              8
#define GPIOA_USB_VBUS              9
#define GPIOA_USB_ID                10
#define GPIOA_USB_DM                11
#define GPIOA_USB_DP                12
#define GPIOA_JTAG_TMS              13
#define GPIOA_JTAG_TCK              14
#define GPIOA_JTAG_TDI              15

#define GPIOB_NC_0                  0
#define GPIOB_NC_1                  1
#define GPIOB_NC_2                  2
#define GPIOB_JTAG_TDO              3
#define GPIOB_JTAG_TRST             4
#define GPIOB_NC_5                  5
#define GPIOB_UART1_TX_EXP          6
#define GPIOB_UART1_RX_EXP          7
#define GPIOB_MOTOR5                8
#define GPIOB_MOTOR6                9
#define GPIOB_I2C2_SCL              10
#define GPIOB_I2C2_SDA              11
#define GPIOB_NRF_CSN               12
#define GPIOB_NRF_SCK               13
#define GPIOB_NC_14                 14
#define GPIOB_PB_CTRL_KILL          15 // active low

#define GPIOC_NRF_CE                0
#define GPIOC_NRF_IRQ               1
#define GPIOC_NRF_MISO              2
#define GPIOC_NRF_MOSI              3
#define GPIOC_NC_4                  4
#define GPIOC_LED_STATUS            5
#define GPIOC_MOTOR1                6
#define GPIOC_MOTOR2                7
#define GPIOC_MOTOR3                8
#define GPIOC_MOTOR4                9
#define GPIOC_MPU6050_INT           10
#define GPIOC_MPU6050_FSYNC         11
#define GPIOC_UART5_TX_CONN         12
#define GPIOC_HMC5883L_DRDY         13 // active low
#define GPIOC_NC_14                 14
#define GPIOC_NC_15                 15

#define GPIOD_UART5_RX_CONN         2

#define GPIOH_OSC_IN                0
#define GPIOH_OSC_OUT               1

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
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))


// GPIO A

#define VAL_GPIOA_MODER     ( PIN_MODE_INPUT(     GPIOA_PB_CTRL_INT     )       \
                            | PIN_MODE_OUTPUT(    GPIOA_BAT_NTC         )       \
                            | PIN_MODE_INPUT(     GPIOA_CHG_STATUS      )       \
                            | PIN_MODE_OUTPUT(    GPIOA_CHG_ISET        )       \
                            | PIN_MODE_INPUT(     GPIOA_NC_4            )       \
                            | PIN_MODE_INPUT(     GPIOA_NC_5            )       \
                            | PIN_MODE_INPUT(     GPIOA_NC_6            )       \
                            | PIN_MODE_INPUT(     GPIOA_NC_7            )       \
                            | PIN_MODE_ALTERNATE( GPIOA_MCO1_CLK        )       \
                            | PIN_MODE_INPUT(     GPIOA_USB_VBUS        )       \
                            | PIN_MODE_INPUT(     GPIOA_USB_ID          )       \
                            | PIN_MODE_ALTERNATE( GPIOA_USB_DM          )       \
                            | PIN_MODE_ALTERNATE( GPIOA_USB_DP          )       \
                            | PIN_MODE_ALTERNATE( GPIOA_JTAG_TMS        )       \
                            | PIN_MODE_ALTERNATE( GPIOA_JTAG_TCK        )       \
                            | PIN_MODE_ALTERNATE( GPIOA_JTAG_TDI        ) )

#define VAL_GPIOA_OTYPER    ( PIN_OTYPE_PUSHPULL( GPIOA_PB_CTRL_INT     )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_BAT_NTC         )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_CHG_STATUS      )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_CHG_ISET        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_NC_4            )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_NC_5            )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_NC_6            )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_NC_7            )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_MCO1_CLK        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_USB_VBUS        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_USB_ID          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_USB_DM          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_USB_DP          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_JTAG_TMS        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_JTAG_TCK        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOA_JTAG_TDI        ) )

#define VAL_GPIOA_OSPEEDR   ( PIN_OSPEED_100M( GPIOA_PB_CTRL_INT     )      \
                            | PIN_OSPEED_100M( GPIOA_BAT_NTC         )      \
                            | PIN_OSPEED_100M( GPIOA_CHG_STATUS      )      \
                            | PIN_OSPEED_100M( GPIOA_CHG_ISET        )      \
                            | PIN_OSPEED_100M( GPIOA_NC_4            )      \
                            | PIN_OSPEED_100M( GPIOA_NC_5            )      \
                            | PIN_OSPEED_100M( GPIOA_NC_6            )      \
                            | PIN_OSPEED_100M( GPIOA_NC_7            )      \
                            | PIN_OSPEED_100M( GPIOA_MCO1_CLK        )      \
                            | PIN_OSPEED_100M( GPIOA_USB_VBUS        )      \
                            | PIN_OSPEED_100M( GPIOA_USB_ID          )      \
                            | PIN_OSPEED_100M( GPIOA_USB_DM          )      \
                            | PIN_OSPEED_100M( GPIOA_USB_DP          )      \
                            | PIN_OSPEED_100M( GPIOA_JTAG_TMS        )      \
                            | PIN_OSPEED_100M( GPIOA_JTAG_TCK        )      \
                            | PIN_OSPEED_100M( GPIOA_JTAG_TDI        ) )

#define VAL_GPIOA_PUPDR     ( PIN_PUPDR_FLOATING( GPIOA_PB_CTRL_INT     )   \
                            | PIN_PUPDR_FLOATING( GPIOA_BAT_NTC         )   \
                            | PIN_PUPDR_PULLUP(   GPIOA_CHG_STATUS      )   \
                            | PIN_PUPDR_FLOATING( GPIOA_CHG_ISET        )   \
                            | PIN_PUPDR_PULLDOWN( GPIOA_NC_4            )   \
                            | PIN_PUPDR_PULLDOWN( GPIOA_NC_5            )   \
                            | PIN_PUPDR_PULLDOWN( GPIOA_NC_6            )   \
                            | PIN_PUPDR_PULLDOWN( GPIOA_NC_7            )   \
                            | PIN_PUPDR_FLOATING( GPIOA_MCO1_CLK        )   \
                            | PIN_PUPDR_FLOATING( GPIOA_USB_VBUS        )   \
                            | PIN_PUPDR_FLOATING( GPIOA_USB_ID          )   \
                            | PIN_PUPDR_FLOATING( GPIOA_USB_DM          )   \
                            | PIN_PUPDR_FLOATING( GPIOA_USB_DP          )   \
                            | PIN_PUPDR_PULLUP(   GPIOA_JTAG_TMS        )   \
                            | PIN_PUPDR_PULLDOWN( GPIOA_JTAG_TCK        )   \
                            | PIN_PUPDR_PULLUP(   GPIOA_JTAG_TDI        ) )

#define VAL_GPIOA_ODR       ( PIN_ODR_LOW( GPIOA_PB_CTRL_INT     )          \
                            | PIN_ODR_LOW( GPIOA_BAT_NTC         )          \
                            | PIN_ODR_LOW( GPIOA_CHG_STATUS      )          \
                            | PIN_ODR_HIGH( GPIOA_CHG_ISET       )          \
                            | PIN_ODR_LOW( GPIOA_NC_4            )          \
                            | PIN_ODR_LOW( GPIOA_NC_5            )          \
                            | PIN_ODR_LOW( GPIOA_NC_6            )          \
                            | PIN_ODR_LOW( GPIOA_NC_7            )          \
                            | PIN_ODR_LOW( GPIOA_MCO1_CLK        )          \
                            | PIN_ODR_LOW( GPIOA_USB_VBUS        )          \
                            | PIN_ODR_LOW( GPIOA_USB_ID          )          \
                            | PIN_ODR_LOW( GPIOA_USB_DM          )          \
                            | PIN_ODR_LOW( GPIOA_USB_DP          )          \
                            | PIN_ODR_LOW( GPIOA_JTAG_TMS        )          \
                            | PIN_ODR_LOW( GPIOA_JTAG_TCK        )          \
                            | PIN_ODR_LOW( GPIOA_JTAG_TDI        ) )

#define VAL_GPIOA_AFRL      ( PIN_AFIO_AF( GPIOA_PB_CTRL_INT     ,  0)      \
                            | PIN_AFIO_AF( GPIOA_BAT_NTC         ,  0)      \
                            | PIN_AFIO_AF( GPIOA_CHG_STATUS      ,  0)      \
                            | PIN_AFIO_AF( GPIOA_CHG_ISET        ,  0)      \
                            | PIN_AFIO_AF( GPIOA_NC_4            ,  0)      \
                            | PIN_AFIO_AF( GPIOA_NC_5            ,  0)      \
                            | PIN_AFIO_AF( GPIOA_NC_6            ,  0)      \
                            | PIN_AFIO_AF( GPIOA_NC_7            ,  0) )

#define VAL_GPIOA_AFRH      ( PIN_AFIO_AF( GPIOA_MCO1_CLK        ,  0)      \
                            | PIN_AFIO_AF( GPIOA_USB_VBUS        ,  0)      \
                            | PIN_AFIO_AF( GPIOA_USB_ID          ,  0)      \
                            | PIN_AFIO_AF( GPIOA_USB_DM          , 10)      \
                            | PIN_AFIO_AF( GPIOA_USB_DP          , 10)      \
                            | PIN_AFIO_AF( GPIOA_JTAG_TMS        ,  0)      \
                            | PIN_AFIO_AF( GPIOA_JTAG_TCK        ,  0)      \
                            | PIN_AFIO_AF( GPIOA_JTAG_TDI        ,  0) )


// GPIO B

#define VAL_GPIOB_MODER     ( PIN_MODE_INPUT(     GPIOB_NC_0           )       \
                            | PIN_MODE_INPUT(     GPIOB_NC_1           )       \
                            | PIN_MODE_INPUT(     GPIOB_NC_2           )       \
                            | PIN_MODE_ALTERNATE( GPIOB_JTAG_TDO       )       \
                            | PIN_MODE_ALTERNATE( GPIOB_JTAG_TRST      )       \
                            | PIN_MODE_INPUT(     GPIOB_NC_5           )       \
                            | PIN_MODE_INPUT(     GPIOB_UART1_TX_EXP   )       \
                            | PIN_MODE_INPUT(     GPIOB_UART1_RX_EXP   )       \
                            | PIN_MODE_ALTERNATE( GPIOB_MOTOR5         )       \
                            | PIN_MODE_ALTERNATE( GPIOB_MOTOR6         )       \
                            | PIN_MODE_INPUT(     GPIOB_I2C2_SCL       )       \
                            | PIN_MODE_INPUT(     GPIOB_I2C2_SDA       )       \
                            | PIN_MODE_OUTPUT(    GPIOB_NRF_CSN        )       \
                            | PIN_MODE_ALTERNATE( GPIOB_NRF_SCK        )       \
                            | PIN_MODE_INPUT(     GPIOB_NC_14          )       \
                            | PIN_MODE_OUTPUT(    GPIOB_PB_CTRL_KILL   ) )

#define VAL_GPIOB_OTYPER    ( PIN_OTYPE_PUSHPULL(  GPIOB_NC_0           )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_NC_1           )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_NC_2           )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_JTAG_TDO       )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_JTAG_TRST      )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_NC_5           )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_UART1_TX_EXP   )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_UART1_RX_EXP   )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_MOTOR5         )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_MOTOR6         )   \
                            | PIN_OTYPE_OPENDRAIN( GPIOB_I2C2_SCL       )   \
                            | PIN_OTYPE_OPENDRAIN( GPIOB_I2C2_SDA       )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_NRF_CSN        )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_NRF_SCK        )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_NC_14          )   \
                            | PIN_OTYPE_PUSHPULL(  GPIOB_PB_CTRL_KILL   ) )

#define VAL_GPIOB_OSPEEDR   ( PIN_OSPEED_100M( GPIOB_NC_0           )      \
                            | PIN_OSPEED_100M( GPIOB_NC_1           )      \
                            | PIN_OSPEED_100M( GPIOB_NC_2           )      \
                            | PIN_OSPEED_100M( GPIOB_JTAG_TDO       )      \
                            | PIN_OSPEED_100M( GPIOB_JTAG_TRST      )      \
                            | PIN_OSPEED_100M( GPIOB_NC_5           )      \
                            | PIN_OSPEED_100M( GPIOB_UART1_TX_EXP   )      \
                            | PIN_OSPEED_100M( GPIOB_UART1_RX_EXP   )      \
                            | PIN_OSPEED_100M( GPIOB_MOTOR5         )      \
                            | PIN_OSPEED_100M( GPIOB_MOTOR6         )      \
                            | PIN_OSPEED_100M( GPIOB_I2C2_SCL       )      \
                            | PIN_OSPEED_100M( GPIOB_I2C2_SDA       )      \
                            | PIN_OSPEED_100M( GPIOB_NRF_CSN        )      \
                            | PIN_OSPEED_100M( GPIOB_NRF_SCK        )      \
                            | PIN_OSPEED_100M( GPIOB_NC_14          )      \
                            | PIN_OSPEED_100M( GPIOB_PB_CTRL_KILL   ) )

#define VAL_GPIOB_PUPDR     ( PIN_PUPDR_PULLDOWN( GPIOB_NC_0           )   \
                            | PIN_PUPDR_PULLDOWN( GPIOB_NC_1           )   \
                            | PIN_PUPDR_PULLDOWN( GPIOB_NC_2           )   \
                            | PIN_PUPDR_FLOATING( GPIOB_JTAG_TDO       )   \
                            | PIN_PUPDR_PULLUP(   GPIOB_JTAG_TRST      )   \
                            | PIN_PUPDR_PULLDOWN( GPIOB_NC_5           )   \
                            | PIN_PUPDR_FLOATING( GPIOB_UART1_TX_EXP   )   \
                            | PIN_PUPDR_FLOATING( GPIOB_UART1_RX_EXP   )   \
                            | PIN_PUPDR_FLOATING( GPIOB_MOTOR5         )   \
                            | PIN_PUPDR_FLOATING( GPIOB_MOTOR6         )   \
                            | PIN_PUPDR_FLOATING( GPIOB_I2C2_SCL       )   \
                            | PIN_PUPDR_FLOATING( GPIOB_I2C2_SDA       )   \
                            | PIN_PUPDR_FLOATING( GPIOB_NRF_CSN        )   \
                            | PIN_PUPDR_FLOATING( GPIOB_NRF_SCK        )   \
                            | PIN_PUPDR_PULLDOWN( GPIOB_NC_14          )   \
                            | PIN_PUPDR_FLOATING( GPIOB_PB_CTRL_KILL   ) )

#define VAL_GPIOB_ODR       ( PIN_ODR_LOW( GPIOB_NC_0           )          \
                            | PIN_ODR_LOW( GPIOB_NC_1           )          \
                            | PIN_ODR_LOW( GPIOB_NC_2           )          \
                            | PIN_ODR_LOW( GPIOB_JTAG_TDO       )          \
                            | PIN_ODR_LOW( GPIOB_JTAG_TRST      )          \
                            | PIN_ODR_LOW( GPIOB_NC_5           )          \
                            | PIN_ODR_LOW( GPIOB_UART1_TX_EXP   )          \
                            | PIN_ODR_LOW( GPIOB_UART1_RX_EXP   )          \
                            | PIN_ODR_LOW( GPIOB_MOTOR5         )          \
                            | PIN_ODR_LOW( GPIOB_MOTOR6         )          \
                            | PIN_ODR_LOW( GPIOB_I2C2_SCL       )          \
                            | PIN_ODR_LOW( GPIOB_I2C2_SDA       )          \
                            | PIN_ODR_HIGH( GPIOB_NRF_CSN       )          \
                            | PIN_ODR_LOW( GPIOB_NRF_SCK        )          \
                            | PIN_ODR_LOW( GPIOB_NC_14          )          \
                            | PIN_ODR_HIGH( GPIOB_PB_CTRL_KILL  ) )

#define VAL_GPIOB_AFRL      ( PIN_AFIO_AF( GPIOB_NC_0           ,  0)      \
                            | PIN_AFIO_AF( GPIOB_NC_1           ,  0)      \
                            | PIN_AFIO_AF( GPIOB_NC_2           ,  0)      \
                            | PIN_AFIO_AF( GPIOB_JTAG_TDO       ,  0)      \
                            | PIN_AFIO_AF( GPIOB_JTAG_TRST      ,  0)      \
                            | PIN_AFIO_AF( GPIOB_NC_5           ,  0)      \
                            | PIN_AFIO_AF( GPIOB_UART1_TX_EXP   ,  7)      \
                            | PIN_AFIO_AF( GPIOB_UART1_RX_EXP   ,  7) )

#define VAL_GPIOB_AFRH      ( PIN_AFIO_AF( GPIOB_MOTOR5         ,  2)      \
                            | PIN_AFIO_AF( GPIOB_MOTOR6         ,  2)      \
                            | PIN_AFIO_AF( GPIOB_I2C2_SCL       ,  4)      \
                            | PIN_AFIO_AF( GPIOB_I2C2_SDA       ,  4)      \
                            | PIN_AFIO_AF( GPIOB_NRF_CSN        ,  0)      \
                            | PIN_AFIO_AF( GPIOB_NRF_SCK        ,  5)      \
                            | PIN_AFIO_AF( GPIOB_NC_14          ,  0)      \
                            | PIN_AFIO_AF( GPIOB_PB_CTRL_KILL   ,  0) )


// GPIO C

#define VAL_GPIOC_MODER     ( PIN_MODE_OUTPUT(    GPIOC_NRF_CE          )       \
                            | PIN_MODE_INPUT(     GPIOC_NRF_IRQ         )       \
                            | PIN_MODE_ALTERNATE( GPIOC_NRF_MISO        )       \
                            | PIN_MODE_ALTERNATE( GPIOC_NRF_MOSI        )       \
                            | PIN_MODE_INPUT(     GPIOC_NC_4            )       \
                            | PIN_MODE_OUTPUT(    GPIOC_LED_STATUS      )       \
                            | PIN_MODE_ALTERNATE( GPIOC_MOTOR1          )       \
                            | PIN_MODE_ALTERNATE( GPIOC_MOTOR2          )       \
                            | PIN_MODE_ALTERNATE( GPIOC_MOTOR3          )       \
                            | PIN_MODE_ALTERNATE( GPIOC_MOTOR4          )       \
                            | PIN_MODE_INPUT(     GPIOC_MPU6050_INT     )       \
                            | PIN_MODE_INPUT(     GPIOC_MPU6050_FSYNC   )       \
                            | PIN_MODE_INPUT(     GPIOC_UART5_TX_CONN   )       \
                            | PIN_MODE_INPUT(     GPIOC_HMC5883L_DRDY   )       \
                            | PIN_MODE_INPUT(     GPIOC_NC_14           )       \
                            | PIN_MODE_INPUT(     GPIOC_NC_15           ) )

#define VAL_GPIOC_OTYPER    ( PIN_OTYPE_PUSHPULL( GPIOC_NRF_CE          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_NRF_IRQ         )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_NRF_MISO        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_NRF_MOSI        )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_NC_4            )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_LED_STATUS      )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_MOTOR1          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_MOTOR2          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_MOTOR3          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_MOTOR4          )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_MPU6050_INT     )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_MPU6050_FSYNC   )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_UART5_TX_CONN   )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_HMC5883L_DRDY   )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_NC_14           )   \
                            | PIN_OTYPE_PUSHPULL( GPIOC_NC_15           ) )

#define VAL_GPIOC_OSPEEDR   ( PIN_OSPEED_100M( GPIOC_NRF_CE          )      \
                            | PIN_OSPEED_100M( GPIOC_NRF_IRQ         )      \
                            | PIN_OSPEED_100M( GPIOC_NRF_MISO        )      \
                            | PIN_OSPEED_100M( GPIOC_NRF_MOSI        )      \
                            | PIN_OSPEED_100M( GPIOC_NC_4            )      \
                            | PIN_OSPEED_100M( GPIOC_LED_STATUS      )      \
                            | PIN_OSPEED_100M( GPIOC_MOTOR1          )      \
                            | PIN_OSPEED_100M( GPIOC_MOTOR2          )      \
                            | PIN_OSPEED_100M( GPIOC_MOTOR3          )      \
                            | PIN_OSPEED_100M( GPIOC_MOTOR4          )      \
                            | PIN_OSPEED_100M( GPIOC_MPU6050_INT     )      \
                            | PIN_OSPEED_100M( GPIOC_MPU6050_FSYNC   )      \
                            | PIN_OSPEED_100M( GPIOC_UART5_TX_CONN   )      \
                            | PIN_OSPEED_100M( GPIOC_HMC5883L_DRDY   )      \
                            | PIN_OSPEED_100M( GPIOC_NC_14           )      \
                            | PIN_OSPEED_100M( GPIOC_NC_15           ) )

#define VAL_GPIOC_PUPDR     ( PIN_PUPDR_FLOATING( GPIOC_NRF_CE          )   \
                            | PIN_PUPDR_PULLUP(   GPIOC_NRF_IRQ         )   \
                            | PIN_PUPDR_PULLDOWN( GPIOC_NRF_MISO        )   \
                            | PIN_PUPDR_FLOATING( GPIOC_NRF_MOSI        )   \
                            | PIN_PUPDR_PULLDOWN( GPIOC_NC_4            )   \
                            | PIN_PUPDR_FLOATING( GPIOC_LED_STATUS      )   \
                            | PIN_PUPDR_FLOATING( GPIOC_MOTOR1          )   \
                            | PIN_PUPDR_FLOATING( GPIOC_MOTOR2          )   \
                            | PIN_PUPDR_FLOATING( GPIOC_MOTOR3          )   \
                            | PIN_PUPDR_FLOATING( GPIOC_MOTOR4          )   \
                            | PIN_PUPDR_PULLDOWN( GPIOC_MPU6050_INT     )   \
                            | PIN_PUPDR_FLOATING( GPIOC_MPU6050_FSYNC   )   \
                            | PIN_PUPDR_FLOATING( GPIOC_UART5_TX_CONN   )   \
                            | PIN_PUPDR_FLOATING( GPIOC_HMC5883L_DRDY   )   \
                            | PIN_PUPDR_PULLDOWN( GPIOC_NC_14           )   \
                            | PIN_PUPDR_PULLDOWN( GPIOC_NC_15           ) )

#define VAL_GPIOC_ODR       ( PIN_ODR_LOW(  GPIOC_NRF_CE          )          \
                            | PIN_ODR_LOW(  GPIOC_NRF_IRQ         )          \
                            | PIN_ODR_LOW(  GPIOC_NRF_MISO        )          \
                            | PIN_ODR_LOW(  GPIOC_NRF_MOSI        )          \
                            | PIN_ODR_LOW(  GPIOC_NC_4            )          \
                            | PIN_ODR_HIGH( GPIOC_LED_STATUS      )          \
                            | PIN_ODR_LOW(  GPIOC_MOTOR1          )          \
                            | PIN_ODR_LOW(  GPIOC_MOTOR2          )          \
                            | PIN_ODR_LOW(  GPIOC_MOTOR3          )          \
                            | PIN_ODR_LOW(  GPIOC_MOTOR4          )          \
                            | PIN_ODR_LOW(  GPIOC_MPU6050_INT     )          \
                            | PIN_ODR_LOW(  GPIOC_MPU6050_FSYNC   )          \
                            | PIN_ODR_LOW(  GPIOC_UART5_TX_CONN   )          \
                            | PIN_ODR_LOW(  GPIOC_HMC5883L_DRDY   )          \
                            | PIN_ODR_LOW(  GPIOC_NC_14           )          \
                            | PIN_ODR_LOW(  GPIOC_NC_15           ) )

#define VAL_GPIOC_AFRL      ( PIN_AFIO_AF( GPIOC_NRF_CE          ,  0)      \
                            | PIN_AFIO_AF( GPIOC_NRF_IRQ         ,  0)      \
                            | PIN_AFIO_AF( GPIOC_NRF_MISO        ,  5)      \
                            | PIN_AFIO_AF( GPIOC_NRF_MOSI        ,  5)      \
                            | PIN_AFIO_AF( GPIOC_NC_4            ,  0)      \
                            | PIN_AFIO_AF( GPIOC_LED_STATUS      ,  0)      \
                            | PIN_AFIO_AF( GPIOC_MOTOR1          ,  2)      \
                            | PIN_AFIO_AF( GPIOC_MOTOR2          ,  2) )

#define VAL_GPIOC_AFRH      ( PIN_AFIO_AF( GPIOC_MOTOR3          ,  2)      \
                            | PIN_AFIO_AF( GPIOC_MOTOR4          ,  2)      \
                            | PIN_AFIO_AF( GPIOC_MPU6050_INT     ,  0)      \
                            | PIN_AFIO_AF( GPIOC_MPU6050_FSYNC   ,  0)      \
                            | PIN_AFIO_AF( GPIOC_UART5_TX_CONN   ,  8)      \
                            | PIN_AFIO_AF( GPIOC_HMC5883L_DRDY   ,  0)      \
                            | PIN_AFIO_AF( GPIOC_NC_14           ,  0)      \
                            | PIN_AFIO_AF( GPIOC_NC_15           ,  0) )


// GPIO D

#define VAL_GPIOD_MODER     ( PIN_MODE_INPUT( GPIOD_UART5_RX_CONN ) )

#define VAL_GPIOD_OTYPER    ( PIN_OTYPE_PUSHPULL( GPIOD_UART5_RX_CONN ) )

#define VAL_GPIOD_OSPEEDR   ( PIN_OSPEED_100M( GPIOD_UART5_RX_CONN ) )

#define VAL_GPIOD_PUPDR     ( PIN_PUPDR_FLOATING( GPIOD_UART5_RX_CONN ) )

#define VAL_GPIOD_ODR       ( PIN_ODR_LOW( GPIOD_UART5_RX_CONN ) )

#define VAL_GPIOD_AFRL      ( PIN_AFIO_AF( GPIOD_UART5_RX_CONN, 8) )

#define VAL_GPIOD_AFRH      ( 0 )


// GPIO E

#define VAL_GPIOE_MODER     ( 0 )

#define VAL_GPIOE_OTYPER    ( 0 )

#define VAL_GPIOE_OSPEEDR   ( 0 )

#define VAL_GPIOE_PUPDR     ( 0 )

#define VAL_GPIOE_ODR       ( 0 )

#define VAL_GPIOE_AFRL      ( 0 )

#define VAL_GPIOE_AFRH      ( 0 )


// GPIO F

#define VAL_GPIOF_MODER     ( 0 )

#define VAL_GPIOF_OTYPER    ( 0 )

#define VAL_GPIOF_OSPEEDR   ( 0 )

#define VAL_GPIOF_PUPDR     ( 0 )

#define VAL_GPIOF_ODR       ( 0 )

#define VAL_GPIOF_AFRL      ( 0 )

#define VAL_GPIOF_AFRH      ( 0 )


// GPIO G

#define VAL_GPIOG_MODER     ( 0 )

#define VAL_GPIOG_OTYPER    ( 0 )

#define VAL_GPIOG_OSPEEDR   ( 0 )

#define VAL_GPIOG_PUPDR     ( 0 )

#define VAL_GPIOG_ODR       ( 0 )

#define VAL_GPIOG_AFRL      ( 0 )

#define VAL_GPIOG_AFRH      ( 0 )


// GPIO H

#define VAL_GPIOH_MODER     ( PIN_MODE_INPUT( GPIOH_OSC_IN      )           \
                            | PIN_MODE_INPUT( GPIOH_OSC_OUT     ) )

#define VAL_GPIOH_OTYPER    ( PIN_OTYPE_PUSHPULL( GPIOH_OSC_IN      )       \
                            | PIN_OTYPE_PUSHPULL( GPIOH_OSC_OUT     ) )

#define VAL_GPIOH_OSPEEDR   ( PIN_OSPEED_100M( GPIOH_OSC_IN      )          \
                            | PIN_OSPEED_100M( GPIOH_OSC_OUT     ) )

#define VAL_GPIOH_PUPDR     ( PIN_PUPDR_FLOATING( GPIOH_OSC_IN      )       \
                            | PIN_PUPDR_FLOATING( GPIOH_OSC_OUT     ) )

#define VAL_GPIOH_ODR       ( PIN_ODR_LOW( GPIOH_OSC_IN      )              \
                            | PIN_ODR_LOW( GPIOH_OSC_OUT     ) )

#define VAL_GPIOH_AFRL      ( PIN_AFIO_AF( GPIOH_OSC_IN    , 0)             \
                            | PIN_AFIO_AF( GPIOH_OSC_OUT   , 0) )

#define VAL_GPIOH_AFRH      ( 0 )


// GPIO I

#define VAL_GPIOI_MODER     ( 0 )

#define VAL_GPIOI_OTYPER    ( 0 )

#define VAL_GPIOI_OSPEEDR   ( 0 )

#define VAL_GPIOI_PUPDR     ( 0 )

#define VAL_GPIOI_ODR       ( 0 )

#define VAL_GPIOI_AFRL      ( 0 )

#define VAL_GPIOI_AFRH      ( 0 )


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif


#define boardInit() board_init()

void board_init(void);

#define ERROR_LEVEL_NORMAL         0
#define ERROR_LEVEL_WARNING        1
#define ERROR_LEVEL_CRITICAL       2
void error_set(int level);
void error_clear(int level);
int error_level_get(void);

bool safemode_active(void);

#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
