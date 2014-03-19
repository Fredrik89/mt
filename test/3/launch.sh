#!/bin/bash

export JOBSIGNALER_DIR=./jobsignaler
mkdir -p $JOBSIGNALER_DIR
rm -f $JOBSIGNALER_DIR/*
rm -f *.log



APP="/home/olle/Git/mt/gtrm/linux/cgroups/bin/application"
RMG="/home/olle/Git/mt/gtrm/linux/cgroups/bin/gtrm"


# initial setup
#echo 0 > /sys/devices/system/cpu/cpu1/online
#echo 0 > /sys/devices/system/cpu/cpu2/online
#echo 0 > /sys/devices/system/cpu/cpu3/online

# start RM
$RMG 0.5 5.0 &
RMGPID=$!

# start applications
$APP 1.0 10000.0 0.0 0.0 0.0 0.2 0.9 0.1 &
AP1PID=$!
sleep 5 
$APP 1.0 5000.0 3000.0 0.0 0.0 0.0 0.7 0.1 &
AP2PID=$!
sleep 5 
$APP 1.0 0.0 20000.0 0.0 0.0 0.0 0.8 0.1 &
AP3PID=$! 
echo "Created applications 1-3"
sleep 5
#kill -2 $AP1PID 
echo "Killed application with lambda 0.1"
sleep 5
$APP 1.0 0.0 50000.0 0.0 0.0 0.0 0.5 0.1 &
AP4PID=$! 
echo "Created application 4"
sleep 5
kill -2 $AP2PID $AP3PID $AP4PID
kill -2 $RMGPID
kill -2 $AP1PID 
# restart cpus
#echo 1 > /sys/devices/system/cpu/cpu1/online
#echo 1 > /sys/devices/system/cpu/cpu2/online
#echo 1 > /sys/devices/system/cpu/cpu3/online

