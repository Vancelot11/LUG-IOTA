MPL3115A2 I2C Sensor Example

This example demonstrates how to use an I2C sensor with the
raspberry pi.  The sensor used in this example is the MPL3115A2
pressure and temperature sensor.  The methods shown here can
be used with a variety of I2C sensors.  In the following
documentation '$USER' is the name of the user under which this
example will be running.  Most likely this will be the 'pi'
user, i.e., USER='pi'.

The contents of this directory include:

  getSensorData.py - a python script that demonstrates how to
      communicate with and get data from an I2C sensor

  mpl3115a2_agent.py - a python script that acts as a agent
      between the mpl3115a2 sensor and web services

  mpl3115a2.html - a web document for displaying data from
      the mpl3115a2 sensor.  This file should be placed in
      a directory named /home/$USER/public_html/mpl3115a2.

  index.html - should be placed in the directory
      /home/$USER/public_html/mpl3115a2

  static - copy this directory and it contents to the 
      directory /home/$USER/public_html/mpl3115a2

  mplstart, mplstop, startup.sh - use these scripts to start
      and stop the mpl3115a2_agent process

Running this example requires that apache2 web services be
installed.  Also the round robin database app rrdtool must
be installed.  Rrdtool may be installed by running

    sudo apt-get update
    sudo apt-get install rrdtool

Install apache2 and the LAMP stack by running

    sudo apt-get update
    sudo apt-get install mariadb-client mariadb-server
    sudo apt-get install php7.0 php7.0-mysql
    sudo apt-get install apache2 apache2-mod-php7.0

Enable apache2 to access the user public_html directory by
running

    sudo a2enmod userdir

Setup the raspberry pi to use RAM for temporary files
and logs by running

    sudo setupTmpfs
    sudo setupApacheEnvvars

Create the following folders

    /home/$USER/bin
    /home/$USER/database
    /home/$USER/log
    /home/$USER/public_html/mpl3115a2
    /home/$USER/public_html/mpl3115a2/dynamic

Add the following line to the file /etc/rc.local

   (su - pi -c "bin/startup.sh")&

Place the following scripts in the /home/$USER/bin directory

    mplstart
    mplstop
    mpl3115a2_agent.py
    startup.sh

Start and stop the mpl3115a2 agent by running mplstart and mplstop.

