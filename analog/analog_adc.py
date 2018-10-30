import spidev

import time

spi = spidev.SpiDev()
spi.open(1, 1)

while True:
    result = spi.xfer([104, 0])
    digital_code = int(((result[0] & 3) << 8) + result[1])
    voltage = round(((digital_code * 0.33) / 1024), 2)
    print voltage
    time.sleep(0.5)
