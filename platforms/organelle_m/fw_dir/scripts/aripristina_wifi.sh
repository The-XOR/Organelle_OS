#!/bin/sh
systemctl stop cherrypy.service
systemctl stop createap.service
echo "Dentro fw_dir/scripts c'e' roba wifi"
python3 /home/music/Organelle_OS/platforms/organelle_m/fw_dir/scripts/wifi_control.py
