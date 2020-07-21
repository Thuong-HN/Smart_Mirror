#!/bin/bash

FILE=/home/pi/gateway/smartmirror.tar
if [ -f "$FILE" ]; then
/bin/tar -xvf $FILE -C /home/pi/gateway/
fi

exit 0