
BOARDSRC = src/board.c
BOARDINC = src/

PROJSRC  = src/main.c
PROJSRC += src/usbcfg.c
PROJSRC += src/exti.c
PROJSRC += src/sensors/mpu60X0.c
PROJSRC += src/sensors/ms5611.c
PROJSRC += src/sensors/hmc5883l.c
PROJSRC += src/sensors/onboardsensors.c
PROJSRC += src/serial-datagram/serial_datagram.c
PROJSRC += src/crc/crc32.c
PROJSRC += src/cmp/cmp.c
PROJSRC += src/shell_commands.c