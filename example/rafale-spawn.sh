#!/bin/sh

ADDRESS=127.0.0.1
PORT=9042
#USER=www-data
#GROUP=www-data
CGI_FILE="./rafale.fcgi"
PID_FILE="$(dirname $CGI_FILE)/$(basename $CGI_FILE).pid"

################################################################################
# DO NOT EDIT BELOW                                                            #
# Unless you know what you are doing, but I seriously, don't                   #
################################################################################

usage() {
  echo "Usage is:\n\t$0 start|stop"; exit
}

start() {
  echo "Sarting spawn-fcgi"
  if [ -f $PID_FILE ]; then
    echo "PID file exists, if YOU ARE SURE cgi is not running remove [$PID_FILE]";
  else
 #   spawn-fcgi -a $ADDRESS -p $PORT -u $USER -g $GROUP -P "$PID_FILE" -f "$CGI_FILE";
   spawn-fcgi -p $PORT -P "$PID_FILE" -f "$CGI_FILE";
  fi
}

stop() {
  echo "Stopping spawn-fcgi"
  if [ ! -f "$PID_FILE" ]; then
    echo "PID file not found, if YOU ARE SURE cgi is running, try force-stop"; 
  else
    kill $(cat "$PID_FILE")
    rm "$PID_FILE" 
  fi
}

force_stop() {
  echo "/!\\ This is likely to kill every rafale cgi running /!\\"
  pkill $(basename "$CGI_FILE")
  rm "$PID_FILE"
}

if [ $# -lt 1 ]; then
  usage
fi

case $1 in
  'start')
    start ;;
  'stop')
    stop ;;
  'restart')
    stop; start ;;
  'force-stop')
    force_stop ;;
esac