
BOARDSRC = src/board.c
BOARDINC = src/

PROJSRC  = src/main.c
PROJSRC += src/usbcfg.c
PROJSRC += src/exti.c
PROJSRC += src/sensors/mpu60X0.c
PROJSRC += src/sensors/ms5611.c
PROJSRC += src/sensors/hmc5883l.c
PROJSRC += src/onboardsensors.c
PROJSRC += src/serial-datagram/serial_datagram.c
PROJSRC += src/crc/crc32.c
PROJSRC += src/cmp/cmp.c
PROJSRC += src/shell_cmd.c
PROJSRC += src/timestamp/timestamp_stm32.c
PROJSRC += src/timestamp/timestamp.c
PROJSRC += src/nrf24l01p.c
PROJSRC += src/radio.c
PROJSRC += src/stream.c
PROJSRC += src/cmp_mem_access/cmp_mem_access.c
PROJSRC += src/ina220.c
PROJSRC += src/motor.c
PROJSRC += src/crc16.c
PROJSRC += src/rate_control.c
PROJSRC += src/pid.c

