#!/bin/bash
set -e
ip neigh show 172.17.0.3 | grep -q 'lladdr de:ad:be:ef:ff:ff' || \
ip neigh add 172.17.0.3 lladdr de:ad:be:ef:ff:ff dev eth0 nud permanent

ip neigh show 172.17.0.4 | grep -q 'lladdr 5f:34:72:70:5f:61' || \
ip neigh add 172.17.0.4 lladdr 5f:34:72:70:5f:61 dev eth0 nud permanent

/usr/sbin/apache2ctl -D FOREGROUND &
sleep 1

service cron start

socat TCP-LISTEN:5000,fork,reuseaddr EXEC:"/run_osquery.sh",pty,stderr & 

tail -f /dev/null
