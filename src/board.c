
#include "hal.h"

#if HAL_USE_PAL
/**
 * @brief     PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *                    This variable is used by the HAL when initializing the PAL driver.
 */
const PALConfig pal_default_config =
{
    {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
     VAL_GPIOA_ODR, VAL_GPIOA_AFRL, VAL_GPIOA_AFRH},
    {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
     VAL_GPIOB_ODR, VAL_GPIOB_AFRL, VAL_GPIOB_AFRH},
    {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
     VAL_GPIOC_ODR, VAL_GPIOC_AFRL, VAL_GPIOC_AFRH},
    {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
     VAL_GPIOD_ODR, VAL_GPIOD_AFRL, VAL_GPIOD_AFRH},
    {VAL_GPIOE_MODER, VAL_GPIOE_OTYPER, VAL_GPIOE_OSPEEDR, VAL_GPIOE_PUPDR,
     VAL_GPIOE_ODR, VAL_GPIOE_AFRL, VAL_GPIOE_AFRH},
    {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
     VAL_GPIOF_ODR, VAL_GPIOF_AFRL, VAL_GPIOF_AFRH},
    {VAL_GPIOG_MODER, VAL_GPIOG_OTYPER, VAL_GPIOG_OSPEEDR, VAL_GPIOG_PUPDR,
     VAL_GPIOG_ODR, VAL_GPIOG_AFRL, VAL_GPIOG_AFRH},
    {VAL_GPIOH_MODER, VAL_GPIOH_OTYPER, VAL_GPIOH_OSPEEDR, VAL_GPIOH_PUPDR,
     VAL_GPIOH_ODR, VAL_GPIOH_AFRL, VAL_GPIOH_AFRH},
    {VAL_GPIOI_MODER, VAL_GPIOI_OTYPER, VAL_GPIOI_OSPEEDR, VAL_GPIOI_PUPDR,
     VAL_GPIOI_ODR, VAL_GPIOI_AFRL, VAL_GPIOI_AFRH}
};
#endif

#define BOOTLOADER_MAGIC_VALUE 0xaabbccdd00112233
static __attribute__((section(".noinit"))) uint64_t execute_bootloader;

void check_bootloader(void)
{
    if (execute_bootloader == BOOTLOADER_MAGIC_VALUE) {
        execute_bootloader = 0;
        __asm__ __volatile__ (
            "LDR  R0, =0x1FFF0000   \n"
            "LDR  SP,[R0, #0]       \n"
            "LDR  R0,[R0, #4]       \n"
            "BX   R0                \n"
        );
    }
}

void reboot_and_run_bootloader(void)
{
    execute_bootloader = BOOTLOADER_MAGIC_VALUE;
    NVIC_SystemReset();
}

void __early_init(void) {
    stm32_clock_init();
}

void board_init(void) {

    // MCO1 as HSE clock output
    RCC->CFGR |= RCC_CFGR_MCO1_1;
    RCC->CFGR &= ~(RCC_CFGR_MCO1_0 | RCC_CFGR_MCO1PRE);

    // expansion UART
    palSetPadMode(GPIOB, GPIOB_UART1_TX_EXP, PAL_MODE_ALTERNATE(7) | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOB, GPIOB_UART1_RX_EXP, PAL_MODE_ALTERNATE(7) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUDR_PULLUP);

    // debug UART
    palSetPadMode(GPIOC, GPIOC_UART5_TX_CONN, PAL_MODE_ALTERNATE(8) | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOD, GPIOD_UART5_RX_CONN, PAL_MODE_ALTERNATE(8) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUDR_PULLUP);

    // sensor i2c bus
    palSetPadMode(GPIOB, GPIOB_I2C2_SCL, PAL_MODE_ALTERNATE(4) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);
    palSetPadMode(GPIOB, GPIOB_I2C2_SDA, PAL_MODE_ALTERNATE(4) | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);

}

void charging_enable(void)
{
    palSetPad(GPIOA, GPIOA_BAT_NTC);
}

void charging_disable(void)
{
    palClearPad(GPIOA, GPIOA_BAT_NTC);
}

static int error_level_cnt[2] = {0,0};

void error_set(int level)
{
    if (level > ERROR_LEVEL_NORMAL && level <= ERROR_LEVEL_CRITICAL) {
        chSysLock();
        error_level_cnt[level - 1]++;
        chSysUnlock();
    }
}

void error_clear(int level)
{
    if (level > ERROR_LEVEL_NORMAL && level <= ERROR_LEVEL_CRITICAL) {
        chSysLock();
        if (error_level_cnt[level - 1] > 0) {
            error_level_cnt[level - 1]--;
        }
        chSysUnlock();
    }
}

int error_level_get(void)
{
    int lvl = ERROR_LEVEL_NORMAL;
    chSysLock();
    if (error_level_cnt[ERROR_LEVEL_CRITICAL - 1]) {
        lvl = ERROR_LEVEL_CRITICAL;
    } else if (error_level_cnt[ERROR_LEVEL_WARNING - 1]) {
        lvl = ERROR_LEVEL_WARNING;
    }
    chSysUnlock();
    return lvl;
}

bool safemode_active(void)
{
    return false;
}
