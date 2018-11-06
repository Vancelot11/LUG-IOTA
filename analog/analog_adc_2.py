from __future__ import division
import RPi.GPIO as GPIO
from time import sleep
import spidev

BUS_CHANNEL = 0
DEVICE_CHANNEL = 0

def bitstring(n):
    s = bin(n)[2:]
    return '0' * (8 - len(s)) + s

def read(adc_channel=0, spi_channel=0):
    conn = spidev.SpiDev(0, spi_channel)
    # conn.open(BUS_CHANNEL, DEVICE_CHANNEL)
    conn.max_speed_hz = 1200000 # 1.2 MHz
    cmd = 128
    if adc_channel:
        cmd += 32
    reply_bytes = conn.xfer([cmd, 0])
    reply_bitstring = ''.join(bitstring(n) for n in reply_bytes)
    reply = reply_bitstring[5:15]
    return int(reply, 2) / 2.0 ** 10.0

if __name__ == '__main__':
    print read()
    print read()

    led_pin = 16
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(led_pin, GPIO.OUT)

    while 0 != 1:
        GPIO.output(led_pin, GPIO.HIGH)
        period = read() * 5
        sleep(period * 0.5)
        GPIO.output(led_pin, GPIO.LOW)
        sleep(period * 0.5)

    GPIO.cleanup()
