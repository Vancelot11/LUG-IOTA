from __future__ import division
import RPi.GPIO as GPIO
from time import sleep
import spidev

BUS_CHANNEL = 0
DEVICE_CHANNEL = 0 


def bitstring(n):
    s = bin(n)[2:]
    return '0' * (8 - len(s)) + s

def read(adc_channel=0):
    conn = spidev.SpiDev()
    conn.open(BUS_CHANNEL, DEVICE_CHANNEL)
    conn.max_speed_hz = 1200000 # 1.2 MHz
    cmd = 128
    if adc_channel:
        cmd += 32
    reply_bytes = conn.xfer([cmd, 0])
    reply_bitstring = ''.join(bitstring(n) for n in reply_bytes)
    reply = reply_bitstring[5:15]
    conn.close()
    return int(reply, 2) / 2.0 ** 10.0

if __name__ == '__main__':
    led_pin = 16
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(led_pin, GPIO.OUT)
    frequency = 50
    dc = 50.9
    p = GPIO.PWM(led_pin, frequency)
    p.start(dc)

    fraks = []

    while 0 != 1:
        try:
            frequency = (read() / 0.005 * 100) + 1 # divide by max value 0.00489 and multiply by the max output 100 adding 1 to make sure frequency is > 0.0
            p.ChangeFrequency(frequency)    
            print frequency
            fraks.append(frequency)
        except KeyboardInterrupt:
            print('Interrupted!')
            break

    print(max(fraks))
    print(min(fraks))

    p.stop()
