#!/bin/sh

env RAFALE_BUILD_TYPE=RELEASE cmake ..
env RAFALE_BUILD_TYPE=RELEASE make
kill -TERM `pgrep -f "./example"`
sleep 1
spawn-fcgi -a 127.0.0.1 -p 9032 -u www-data -g www-data -f ./example
