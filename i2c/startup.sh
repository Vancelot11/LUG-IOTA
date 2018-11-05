#!/bin/sh
#
# Create directories in tmpfs for dynamic data.  Bind the directories
# to their respective paths in the public_html directory.  This assures
# that dynamic content gets written to ram.

# Create a directory in tmpfs for mpl3115a2 dynamic data. Mount the
# directory to the mpl3115a2/dynamic directory in the public_html document
# root directory.
mkdir /tmp/mpl3115a2
sudo mount --bind /tmp/mpl3115a2 /home/pi/public_html/mpl3115a2/dynamic

# start mpl3115a2 device agent
(sleep 5; /home/pi/bin/mplstart;) &

