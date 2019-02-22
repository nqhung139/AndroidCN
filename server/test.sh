#!/bin/bash

. func.h
while [ 1 == 1 ] 
do
    getVersionAdb
    getIPAddress
    getListDevices

    sleep 3
    echo 'puck'
done
