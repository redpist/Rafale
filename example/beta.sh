#!/bin/sh

cmake ..
make
kill -TERM `pgrep -f "./example-beta"`
sleep 1
spawn-fcgi -a 127.0.0.1 -p 9042 -u www-data -g www-data -f ./example
