from __future__ import division
import spidev

def bitstring(n):
    s = bin(n)[2:]
    return '0' * (8 - len(s)) + s

def read(adc_channel=0, spi_channel=0):
    conn = spidev.SpiDev(0, spi_channel)
    conn.open(0, 0)
    conn.max_speed_hz = 1200000 # 1.2 MHz
    cmd = 128
    if adc_channel:
        cmd += 32
    reply_bytes = conn.xfer([cmd, 0])
    reply_bitstring = ''.join(bitstring(n) for n in reply_bytes)
    reply = reply_bitstring[5:15]
    return int(reply, 2) / 2 ** 10

if __name__ == '__main__':
    print read()
