#!/usr/bin/python -u
## The -u option turns off block buffering of python output. This assures
## that error messages get printed to the log file as they happen.
#  
# Module: mpl3115a2_agent.py
#
# Description: This module acts as an agent between the mpl3115a2 sensor and
# the web services.  The agent
#     - converts units of various sensor data items
#     - updates a round robin (rrdtool) database with the sensor data
#     - periodically generate graphic charts for display in html documents
#     - writes the processed sensor data to a JSON file for use by
#       html documents
#
# Note: _rrdtool_ must be installed on the host running this program.
#
# Install rrdtool on the raspberry pi by running
#
#     sudo apt-get install rrdtool
# 
# Create a directory /home/$USER/database for the rrdtool database.
# For more information on rrdtool see http://oss.oetiker.ch/rrdtool
#       
# Copyright 2018 Jeff Owrey
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public Licensef
#    along with this program.  If not, see http://www.gnu.org/license.
#
# Revision History
#   * v10 - 05 Nov 2018 by J L Owrey; first release

import os
import sys
import subprocess
import multiprocessing
import time
import json
import smbus

    ### FILE AND FOLDER LOCATIONS ###

# get the user running this script
_USER = os.environ['USER']
# html document root directory
_DOCROOT_DIRECTORY = '/home/%s/public_html/mpl3115a2/' % _USER
# location of charts used by html documents
_CHARTS_DIRECTORY = _DOCROOT_DIRECTORY + 'dynamic/'
# data from the mpl3115a2 sensor for use by html documents
_OUTPUT_DATA_FILE = _DOCROOT_DIRECTORY + 'dynamic/mpl3115a2Data.js'
# rrdtool database file
_RRD_FILE = '/home/%s/database/mpl3115a2_data.rrd' % _USER

    ### GLOBAL CONSTANTS ###

# web page data item refresh rate (sec)
_SENSOR_POLLING_INTERVAL = 5
# rrdtool database update rate (sec)
_DATABASE_UPDATE_INTERVAL = 10
# generation rate of day charts (sec)
_CHART_UPDATE_INTERVAL = 60
# standard chart width in pixels
_CHART_WIDTH = 600
# standard chart height in pixels
_CHART_HEIGHT = 150
# days of data to store in round robin database
_RRD_SIZE_IN_DAYS = 2 # days


   ### CONVERSION FACTORS ###

# inches Hg per Pascal
_PASCAL_CONVERSION_FACTOR = 0.29530099194
# station elevation above sea level
_STATION_ELEVATION = 0

   ### GLOBAL VARIABLES ###

# turns on or off extensive debugging messages
debugOption = False

    ### HELPER FUNCTIONS ###

def getTimeStamp():
    """
    Sets the error message time stamp to the local system time.
    Parameters: none
    Returns string containing the time stamp.
    """
    return time.strftime('%m/%d/%Y %H:%M:%S', time.localtime())
##end def

def getEpochSeconds(sTime):
    """
    Convert the time stamp supplied in the sensor data string
    to seconds since 1/1/1970 00:00:00.
    Parameters: 
       sTime - the time stamp to be converted must be formatted
               as %m/%d/%Y %H:%M:%S
    Returns epoch seconds.
    """
    try:
        t_sTime = time.strptime(sTime, '%m/%d/%Y %H:%M:%S')
    except Exception, exError:
        print '%s getEpochSeconds: %s' % (getTimeStamp(), exError)
        return None
    tSeconds = int(time.mktime(t_sTime))
    return tSeconds
##end def

    ### PUBLIC FUNCTIONS ###

def getSensorData(dData):
    """
    Get altitude, pressure, and temperature from the MPL3115A2
    sensor.
    Parameters:
        dData - a dictionary object to contain sensor data
    Returns:
        True if successful, False otherwise
    """
    
    # Define device bus address and registers
    MPL3115A2_ADDRESS = 0x60
    CTRL_REG1 = 0x26
    PT_DATA_CFG = 0x13
    OUT_DATA_REG = 0x00

    try:
        # Create an I2C bus object
        bus = smbus.SMBus(1)

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

        # Read data back from OUT_DATA_REG, 6 bytes returned: status,
        # altitude MSB, altitude CSB, altitude LSB, temp MSB, temp LSB.
        data = bus.read_i2c_block_data(MPL3115A2_ADDRESS, OUT_DATA_REG, 6)

        # Convert the data to 20 bit signed number Q16.4 format.
        altitude = (data[1] << 24 | data[2] << 16 | data[3] << 8) / 65536.0
        # Convert to signed floating point number
        if altitude > (1 << 15):
            altitude -= (1 << 16)

        # Convert the data to a 12 bit signed number Q12.4 format.
        tempCelsius = (data[4] << 8 | data[5]) / 256.0
        # Convert to signed floating point number
        if tempCelsius > (1 << 11):
            tempCelsius -= (1 << 12)

        # Get pressure data

        # Write data to control register CTL_REG1 
        #               10111001 0xB9
        #   |      00        |  111   |      001    |
        #   | barometer mode | OSR128 | active mode |
        bus.write_byte_data(MPL3115A2_ADDRESS, CTRL_REG1, 0x39)

        time.sleep(1.0)

        # Read data back from OUT_DATA_REG, 4 bytes:
        # status, pres MSB1, pres MSB, pres LSB.
        data = bus.read_i2c_block_data(MPL3115A2_ADDRESS, OUT_DATA_REG, 4)

        # Convert the data to 20-bits unsigned number Q18.2 format.
        pressure = (data[1] << 16 | data[2] << 8 | data[3]) / 64.0
        pressure /= 1000.0 # Convert to kilo pascals
    except Exception, exError:
        dData['status'] = 'offline'
        writeOutputDataFile(dData, _OUTPUT_DATA_FILE)
        print '%s getSensorData: %s' % (getTimeStamp(), exError)
        return False

    dData['altitude'] = altitude
    dData['pressure'] = pressure
    dData['temperature'] = tempCelsius
    dData['status'] = 'online'
 
    if debugOption:
        print "Pressure: %.2f kPa" % pressure
        print "Altitude: %.2f m" % altitude
        print "Temperature: %.2f C\n" % tempCelsius

    return True
##end def

def convertData(dData):
    """
    Convert individual sensor data items as necessary.  Also
    format data items for use by html documents.
    Parameters:
       dData - a dictionary object containing the data items to be
               converted
    Returns true if successful, false otherwise.
    """
    try:
        # Validate altitude data
        altitude = dData['altitude']
        if altitude < -10000.0 or altitude > 10000.0:
            raise Exception('invalid altitude: %.4e' % altitude)
        dData['altitude'] = '%.1f' % altitude

        # Validate pressure data
        pressure = dData['pressure']
        if pressure < 0.0 or pressure > 200.0:
            raise Exception('invalid pressure: %.4e' % pressure)
        # Convert pressure from pascals to inches Hg.
        #pressureBar = pressure * _PASCAL_CONVERSION_FACTOR
        #dData['pressure'] = '%.4f' % pressureBar # format for web page
        dData['pressure'] = '%.4f' % pressure

        # Validate temperature data
        temperature = dData['temperature']
        if temperature < -100.0 or temperature > 200.0:
            raise Exception('invalid temperature: %.4e' % altitude)
        # Convert temperature from Celsius to Fahrenheit
        #tempf = 1.8 * dData['temperature'] + 32.0
        #dData['temperature'] = '%.2f' % tempf
        dData['temperature'] = '%.2f' % temperature
    except Exception, exError:
        # Trap any data conversion errors.
        print '%s convertData: %s' % (getTimeStamp(), exError)
        print readInputDataFile()
        return False

    return True
##end def

def writeOutputDataFile(dData, sOutputDataFile):
    """
    Writes to a file a formatted string containing the sensor data.
    The file is written to the document dynamic data folder for use
    by html documents.
    Parameters: 
        dData - dictionary object containing sensor data
    Returns true if successful, false otherwise
    """
    # Set date item to current date and time.
    dData['date'] = getTimeStamp()

    # Format the sensor data as string using java script object notation.
    sData = '[{'
    for key in dData:
        sData += '\"%s\":\"%s\",' % (key, dData[key])
    sData = sData[:-1] + '}]\n'

    # Write the string to the output data file for use by html documents.
    try:
        fc = open(sOutputDataFile, 'w')
        fc.write(sData)
        fc.close()
    except Exception, exError:
        print '%s writeOutputDataFile: %s' % (getTimeStamp(), exError)
        return False

    return True
##end def

    ### DATABASE FUNCTIONS ###

def updateDatabase(dData):
    """
    Updates the rrdtool round robin database with data supplied in
    the sensor data string.
    Parameters:
        dData - dictionary object containing items to be written to the
                rrdtool database
    Returns true if successful, false otherwise.
    """
   # Get the data items to be stored in rrdtool database, and
   # convert string items to floating point numbers as necessary.
    try:
        time = getEpochSeconds(dData['date'])
        altitude = float(dData['altitude'])
        pressure = float(dData['pressure'])
        temperature = float(dData['temperature'])
    # Trap any data conversion errors.
    except Exception, exError:
        print '%s updateDatabase error: %s' % (getTimeStamp(), exError)
        return False

    # Format the rrdtool update command.
    strCmd = 'rrdtool update %s %s:%s:%s:%s' % \
                  (_RRD_FILE, time, altitude, pressure, temperature)
    if debugOption:
        print '%s\n' % strCmd # DEBUG
        pass

    # Run the formatted command as a subprocess.
    try:
        subprocess.check_output(strCmd, stderr=subprocess.STDOUT, \
                                shell=True)
    except subprocess.CalledProcessError, exError:
        print '%s rrdtool update failed: %s' % \
              (getTimeStamp(), exError.output)
        return False
    else:
        if debugOption:
            print 'database update successful'
    return True
##end def

def createAutoGraph(fileName, dataItem, gLabel, gTitle, gStart,
                    lower, upper, addTrend, autoScale):
    """
    Uses rrdtool to create a graph of specified sensor data item.
    Graphs are for display in html documents.
    Parameters:
        fileName - name of graph file
        dataItem - the sensor data item to be graphed
        gLabel - string containing a graph label for the data item
        gTitle - string containing a title for the graph
        gStart - time from now when graph starts
        lower - lower bound for graph ordinate
        upper - upper bound for graph ordinate
        addTrend - 0, show only graph data
                   1, show only a trend line
                   2, show a trend line and the graph data
        autoScale - if True, then use vertical axis auto scaling
            (lower and upper parameters are ignored), otherwise use
            lower and upper parameters to set vertical axis scale
    Returns true if successful, false otherwise.
    """
    # Set the file name for the chart.
    gPath = _CHARTS_DIRECTORY + fileName + '.png'

    # Format the rrdtool create graph command.

    # Set chart start time, height, and width.
    strCmd = 'rrdtool graph %s -a PNG -s %s -e \'now\' -w %s -h %s ' \
             % (gPath, gStart, _CHART_WIDTH, _CHART_HEIGHT)
   
    # Set the range and scaling of the chart y-axis.
    if lower < upper:
        strCmd  +=  '-l %s -u %s -r ' % (lower, upper)
    elif autoScale:
        strCmd += '-A '
    strCmd += '-Y '

    # Set the chart ordinate label and chart title. 
    strCmd += '-v %s -t %s ' % (gLabel, gTitle)

    # Show the data, or a moving average trend line, or both.
    strCmd += 'DEF:dSeries=%s:%s:AVERAGE ' % (_RRD_FILE, dataItem)
    if addTrend == 0:
        strCmd += 'LINE2:dSeries#0400ff '
    elif addTrend == 1:
        strCmd += 'CDEF:smoothed=dSeries,86400,TREND LINE2:smoothed#0400ff '
    elif addTrend == 2:
        strCmd += 'LINE1:dSeries#0400ff '
        strCmd += 'CDEF:smoothed=dSeries,86400,TREND LINE2:smoothed#0400ff '
    
    if debugOption:
        print '%s\n' % strCmd # DEBUG
        pass
    
    # Run the formatted rrdtool command as a subprocess.
    try:
        result = subprocess.check_output(strCmd, \
                     stderr=subprocess.STDOUT,   \
                     shell=True)
    except subprocess.CalledProcessError, exError:
        print 'rrdtool graph failed: %s' % (exError.output)
        return False

    if debugOption:
        print 'rrdtool graph: %s' % result
    return True
##end def

def generateDayGraphs():
    """
    Generate graphs for html documents. Calls createGraph for each graph
    that needs to be created.
    Parameters: none
    Returns nothing.
    """
    createAutoGraph('1d_altitude', 'altitude', 'meters', \
                'Altitude', 'now-1d', 0, 0, 0, True)
    createAutoGraph('1d_pressure', 'pressure', 'kiloPascals', \
                'Barometric\ Pressure', 'now-1d', 0, 0, 0, True)
    createAutoGraph('1d_temperature', 'temperature', 'degrees\ Celsius', \
                'Temperature', 'now-1d', 0, 0, 0, True)
##end def

def createRrdDatabase():
    """
    Creates a rrdtool round robin database.  The database when
    first created does not contain any data.  The database will be
    created in the /home/$USER/database folder.
    Parameters: none
    Returns: true, if successful, false otherwise
    """
    # Creating a database wipes out any previous database with the
    # same name, in the same location.  Require an existing database
    # to be manually deleted before creating a new database.
    if os.path.exists(_RRD_FILE):
        print "rrdtool mpl3115a2 database already exists!"
        return False

    # Calculate the database size
    heartBeat = 2 * _DATABASE_UPDATE_INTERVAL
    rrdNumRows = int(_RRD_SIZE_IN_DAYS                        \
                 * round(86400 / _DATABASE_UPDATE_INTERVAL))
       
    # Format the rrdtool create database command.
    strFmt = ("rrdtool create %s --step %s "
             "DS:altitude:GAUGE:%s:U:U DS:pressure:GAUGE:%s:U:U "
             "DS:temperature:GAUGE:%s:U:U "
             "RRA:AVERAGE:0.5:1:%s")

    strCmd = strFmt % (_RRD_FILE, _DATABASE_UPDATE_INTERVAL, \
                 heartBeat, heartBeat, heartBeat, rrdNumRows)
    
    print "creating rrdtool database...\n\n%s\n" % strCmd # DEBUG

    # Run the formatted command in a subprocess.
    try:
        subprocess.check_output(strCmd, stderr=subprocess.STDOUT, \
                                shell=True)
    except subprocess.CalledProcessError, exError:
        print "rrdtool create failed: %s" % (exError.output)
        return False
    else:
        if debugOption:
            print 'database creation successful\n'
    return True
##end def

def getCLarguments():
    """
    Get command line arguments. There is one possible argument
        -d turns on debug mode
    Returns nothing.
    """
    global debugOption

    index = 1
    while index < len(sys.argv):
        if sys.argv[index] == '-d':
            debugOption = True
        else:
            cmd_name = sys.argv[0].split('/')
            print 'Usage: %s {-d}' % cmd_name[-1]
            exit(-1)
        index += 1
    return
##end def

     ### MAIN ROUTINE ###

def main():
    """
    Executive routine which manages timing and execution of all other
    events.
    Parameters: none
    Returns nothing.
    """
    # Log agent process startup time.
    print '%s starting up mpl3115a2 agent process' % \
                  (getTimeStamp())

    # Last time the data file to the web server updated
    lastCheckForUpdateTime = -1
    # Last time day charts were generated
    lastDayChartUpdateTime = -1
    # Last time the rrdtool database updated
    lastDatabaseUpdateTime = -1

    # Get any command line arguments.
    getCLarguments()

    # Create an rrdtool database if it does not exist.
    if not os.path.exists(_RRD_FILE):
        createRrdDatabase()
 
    # Main loop
    while True:

        currentTime = time.time() # get current time in seconds

        # Every update period, get and process mpl3115a2 sensor data.
        if currentTime - lastCheckForUpdateTime > _SENSOR_POLLING_INTERVAL:
            lastCheckForUpdateTime = currentTime
            dData = {}

            result = getSensorData(dData)

            # If the sensor is online and the data successfully parsed, 
            # then convert the data.
            if result:
                result = convertData(dData)

            # If the data successfully converted, then the write the data
            # to the output data file.
            if result:
               writeOutputDataFile(dData, _OUTPUT_DATA_FILE)

               # At the rrdtool database update interval write the data to
               # the rrdtool database.
               if currentTime - lastDatabaseUpdateTime > \
                       _DATABASE_UPDATE_INTERVAL:   
                   lastDatabaseUpdateTime = currentTime
                   # Update the round robin database with the parsed data.
                   result = updateDatabase(dData)

        # At the day chart generation interval generate day charts.
        if currentTime - lastDayChartUpdateTime > _CHART_UPDATE_INTERVAL:
            lastDayChartUpdateTime = currentTime
            p = multiprocessing.Process(target=generateDayGraphs, args=())
            p.start()

        # Relinquish processing back to the operating system until
        # the next update interval.  Also provide processing time
        # information for debugging and performance analysis.

        elapsedTime = time.time() - currentTime
        if debugOption:
            print 'processing time: %6f sec\n' % elapsedTime
        remainingTime = _SENSOR_POLLING_INTERVAL - elapsedTime
        if remainingTime > 0.0:
            time.sleep(remainingTime)
    ## end while
    return
##end def

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print '\n%s terminating mpl3115a2 agent process' % \
              (getTimeStamp())
        if os.path.exists(_OUTPUT_DATA_FILE):
            os.remove(_OUTPUT_DATA_FILE)

##end module
