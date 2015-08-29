.PHONY: flash
flash: all
	openocd -f openocd.cfg -c "program build/$(PROJECT).elf verify reset" -c "shutdown"

.PHONY: r
r: reset
.PHONY: reset
reset:
	openocd -f openocd.cfg -c "init" -c "reset" -c "shutdown"
