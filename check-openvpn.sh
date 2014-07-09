#! /bin/bash

new_status=$(curl -sS http://s.equalteam.net/openvpn.status)

if [[ "$new_status" = off ]]; then
    if /etc/init.d/openvpn status | grep -q "is running" ; then
        /etc/init.d/openvpn stop
    fi
fi
if [[ "$new_status" = on ]]; then
    if /etc/init.d/openvpn status | grep -q "is not running" ; then
        /etc/init.d/openvpn start
    fi
fi