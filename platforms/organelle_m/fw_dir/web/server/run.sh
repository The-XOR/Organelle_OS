#!/bin/sh


export FW_DIR=${FW_DIR:="/home/patch/fw_dir"}
export SCRIPTS_DIR=$FW_DIR/scripts
export USER_DIR=`$SCRIPTS_DIR/get-user-dir.sh`
echo using USER_DIR: $USER_DIR

export PYTHONPATH="/home/patch/.local/lib/python2.7/site-packages"

# start webserver
cd /home/patch/fw_dir/web/server
python2 server.py 8080 
