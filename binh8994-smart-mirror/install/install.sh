#!/bin/bash

cp -r ../32/ /home/pi/gateway/
cp -r ../256/ /home/pi/gateway/
cp -r ../512/ /home/pi/gateway/

cp ../gateway/gateway /home/pi/gateway/
cp uncompress.sh /home/pi/gateway/

chmod +x /home/pi/gateway/gateway
chmod +x /home/pi/gateway/uncompress.sh

exit 0