#!/usr/bin/python

# This python program gets altitude, pressure, and temperature data
# from the MPL3115A2 I2C sensor.

# Import the I2C interface library
import smbus
import time

# Define constants

MPL3115A2_ADDRESS = 0x60
CTRL_REG1 = 0x26
PT_DATA_CFG = 0x13
OUT_DATA_REG = 0x00

# Create an I2C bus object
bus = smbus.SMBus(1)

def poll():
  init_time = time.time()
  while True:
    data = bus.read_i2c_block_data(MPL3115A2_ADDRESS, OUT_DATA_REG, 1)
    dataReady = data[0] & 0x08
    if dataReady:
      break
    if abs(time.time() - init_time) > 2.0:
      break
    time.sleep(0.01)

# Reboot the MPL3115A2
#bus.write_byte_data(MPL3115A2_ADDRESS, CTRL_REG1, 0x05)

# Get altitude and temperature data

# Write data to control register CTL_REG1 
#               10111001 0xB9
#   |      10        |  111   |      001    |
#   | altimeter mode | OSR128 | active mode |
bus.write_byte_data(MPL3115A2_ADDRESS, CTRL_REG1, 0xB9)

# Write data to data configuration register PT_DATA_CFG (0x13)
#		          00000111 0x07
#   | 00000 |  1   |   1   |   1   |
#   |   X   | DREM | PDEFE | TDEFE |
#
#   X - don't care
#   DREM - Data ready event mode for all events
#   PDEFE - Pressure/Altitude data ready event flag
#   TDEFE - Temperature data ready event flag
bus.write_byte_data(MPL3115A2_ADDRESS, PT_DATA_CFG, 0x07)

time.sleep(1.0)

# Read data back from OUT_DATA_REG, 6 bytes returned:
# status, altitude MSB, altitude CSB, altitude LSB, temp MSB, temp LSB
data = bus.read_i2c_block_data(MPL3115A2_ADDRESS, OUT_DATA_REG, 6)

# Convert the data to 20 bit signed number Q16.4
altitude = (data[1] << 24 | data[2] << 16 | data[3] << 8) / 65536.0
# Convert to signed floating point number
if altitude > (1 << 15):
  altitude -= (1 << 16)

# Convert the data to a 12 bit signed number Q12.4
tempCelsius = (data[4] << 8 | data[5]) / 256.0
# Convert to signed floating point number
if tempCelsius > (1 << 11):
  tempCelsius -= (1 << 12)
# Convert Celsius to Fahrenheit
tempFahr = tempCelsius * 1.8 + 32.0

# Get pressure data

# Write data to control register CTL_REG1 
#               10111001 0xB9
#   |      00        |  111   |      001    |
#   | barometer mode | OSR128 | active mode |
bus.write_byte_data(MPL3115A2_ADDRESS, CTRL_REG1, 0x39)

time.sleep(1.0)

# Read data back from OUT_DATA_REG, 4 bytes:
# status, pres MSB1, pres MSB, pres LSB
data = bus.read_i2c_block_data(MPL3115A2_ADDRESS, OUT_DATA_REG, 4)

# Convert the data to 20-bits unsigned number Q18.2 format
pressure = (data[1] << 16 | data[2] << 8 | data[3]) / 64.0
pressure /= 1000.0 # Convert to kilo pascals

# Output data to screen

print "Pressure : %.2f kPa" % pressure
print "Altitude : %.2f m" % altitude
print "Temperature in Celsius  : %.2f C" % tempCelsius
print "Temperature in Fahrenheit  : %.2f F" % tempFahr

