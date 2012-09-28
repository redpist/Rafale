#!/bin/sh

env RAFALE_BUILD_TYPE=RELEASE cmake ..
env RAFALE_BUILD_TYPE=RELEASE make
kill `pgrep -f "./example"`
sleep 1
spawn-fcgi -a 127.0.0.1 -p 9041 -u www-data -g www-data -f ./example
