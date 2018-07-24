#!/bin/bash
sudo modprobe bluetooth_6lowpan

echo "0" | sudo tee /sys/kernel/debug/bluetooth/6lowpan_enable
sleep 1
echo "1" | sudo tee /sys/kernel/debug/bluetooth/6lowpan_enable

echo "connect 00:56:7B:01:07:8C 1" | sudo tee /sys/kernel/debug/bluetooth/6lowpan_control

echo "1" | sudo tee  /proc/sys/net/ipv6/conf/all/forwarding
sleep 2

while (! ifconfig bt0); do
        sleep 1
done

sudo ifconfig bt0 add fd00:a::1/64
sudo /etc/init.d/radvd restart