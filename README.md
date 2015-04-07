# Nanocopter

## threads:
- sensor: reading sensors on the I2C bus
- radio: remote control, parameter, logging, ...
- control: simple rotation rate PID
- div: shell via radio or uart

## radio protocol

Nordic nRF24L01+ radio transceiver

Enhanced ShockBurst data link configuration:
- 3byte MAC
- 2byte CRC
- dynamic payload length, 1-32 bytes
- ack payload

1 byte header, 0 - 31 bytes data.

header bits:
[0-3]: port
[4-7]: sequence number, increments for every packet addressed to the corresponding port.

- port 0 has highest proirity, port 15 lowest.
- no guarantees for data arrival.
- packet has a sequence number for lost or duplicate packet detection.

### nanocopter radio port assignment

Port | Application
---- | -----------
0    | remote control commands
1    | parameter service
3    | data log
4    | shell
5    | firmware upgrade
...  | ...
14   | debug channel
15   | communication control & monitoring (ping, ...)
