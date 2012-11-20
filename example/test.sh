#!/usr/bin/zsh


export CONTENT_LENGTH=""
export CONTENT_TYPE=""
export FCGI_ROLE="RESPONDER"
export GATEWAY_INTERFACE="CGI/1.1"
export HTTP_ACCEPT="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
export HTTP_ACCEPT_CHARSET="ISO-8859-1,utf-8;q=0.7,*;q=0.3"
export HTTP_ACCEPT_ENCODING="gzip,deflate,sdch"
export HTTP_ACCEPT_LANGUAGE="en-US,en;q=0.8,fr;q=0.6"
export HTTP_CONNECTION="keep-alive"
export HTTP_COOKIE="sessionid=37517340320838272131525932197"
export HTTP_HOST="192.168.1.26"
export HTTP_USER_AGENT="Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11"
export PATH_INFO="$1"
export QUERY_STRING="$2"
export REMOTE_ADDR="192.168.1.26"
export REMOTE_PORT="51042"
export REQUEST_METHOD="GET"
export SCRIPT_FILENAME="$1"
export SERVER_ADDR="192.168.1.26"
export SERVER_NAME="nash"
export SERVER_PORT="80"
export SERVER_PROTOCOL="HTTP/1.1"
export SERVER_SOFTWARE="nginx/1.0.5"

make && valgrind ./example-local

