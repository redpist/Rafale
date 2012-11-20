#!/bin/sh

env RAFALE_BUILD_TYPE=LOCAL cmake ..
env RAFALE_BUILD_TYPE=LOCAL make
kill -TERM `pgrep -f "./example-local"`
sleep 1
spawn-fcgi -a 127.0.0.1 -p 9032 -u www-data -g www-data -f ./example-local
