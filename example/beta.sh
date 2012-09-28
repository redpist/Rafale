#!/bin/sh

env RAFALE_BUILD_TYPE=BETA cmake ..
env RAFALE_BUILD_TYPE=BETA make
kill -TERM `pgrep -f "./example-beta"`
sleep 1
spawn-fcgi -a 127.0.0.1 -p 9041 -u www-data -g www-data -f ./example-beta
