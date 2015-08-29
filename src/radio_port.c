#include <hal.h>

SPIDriver *spi_init(void)
{
    /*
     * SPI2 configuration structure.
     * SPI2 is on APB1 @ 42MHz / 4 = 10.5 MHz
     * CPHA=0, CPOL=0, 8bits frames, MSb transmitted first.
     */
    static SPIConfig spi_cfg = {
        .end_cb = NULL,
        .ssport = GPIOB,
        .sspad = GPIOB_NRF_CSN,
        .cr1 =  SPI_CR1_BR_0
    };
    spiStart(&SPID2, &spi_cfg);
    return &SPID2;
}

void nrf_ce_active(void)
{
    palSetPad(GPIOC, GPIOC_NRF_CE);
}

void nrf_ce_inactive(void)
{
    palClearPad(GPIOC, GPIOC_NRF_CE);
}