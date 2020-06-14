#!/usr/bin/python3


import time
import struct
import random

import serial


dev = serial.Serial('/dev/serial/by-id/usb-tensor-robotics_MOTOR_DRV_2049B8385941-if00', 10000000, timeout=0.01)
while 1:
    cmd = b'bU\x00\x0a'
    for i in range(30):
        cmd += chr(random.randint(0,0xf)).encode('latin-1')
    dev.write(cmd)
    dev.write(struct.pack('h', 0x5552))
    time.sleep(0.05)
    

