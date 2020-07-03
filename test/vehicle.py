#!/usr/bin/python3

import time
import struct
import serial

class Vecicle( object ):

    CMD_RESET = 0x0052
    CMD_REBOOT = 0x0053
    CMD_M_SPD = 0x4D20
    CMD_ADC = 0x5572

    def __init__(self, dev_name='/dev/serial/by-id/usb-tensor-robotics_MOTOR_DRV_2049B8385941-if00'):
        
        self.dev_name = dev_name
        self.open_dev()
    
    def open_dev(self):

        self.dev = serial.Serial(self.dev_name, 10000000, timeout=0.01)
    
    def motors_spd(self, m1, m2, m3, m4):

        print(m1, m2, m3, m4)
        cmd = struct.pack('hhhhh', self.CMD_M_SPD, m1, m2, m3, m4)
        self.dev.write(cmd)
    
    def move_fb(self, spd):
        self.motors_spd(spd, -spd, -spd, spd)

    def move_lr(self, spd):
        self.motors_spd(-spd, -spd, spd, spd)

    def move_rotate(self, spd):
        self.motors_spd(-spd, -spd, -spd, -spd)
    
    def move_diag(self, d, spd):
        if d == 'LF':
            self.motors_spd(0, -spd, 0, spd)
        elif d == 'RF':
            self.motors_spd(spd, 0, -spd, 0)
        elif d == 'LB':
            self.motors_spd(-spd, 0, spd, 0)
        elif d == 'RB':
            self.motors_spd(0, spd, 0, -spd)
        else:
            self.motors_spd(0, 0, 0, 0)

    def move_turn(self, d, spd):
        if d == 'LF':
            self.motors_spd(-spd, -spd, 0, 0)
        elif d == 'RF':
            self.motors_spd(spd, spd, 0, 0)
        elif d == 'RB':
            self.motors_spd(0, 0, spd, spd)
        elif d == 'LB':
            self.motors_spd(0, 0, -spd, -spd)
    
    def move_mecanum(self, x, y, r):
        spd1 = y - x - r
        spd4 = y + x - r
        spd3 = y - x + r
        spd2 = y + x + r

        spd2 = -spd2
        spd3 = -spd3

        self.motors_spd(spd1, spd2, spd3, spd4)


    def set_pid(self, p, i, d):
        self.dev.write(struct.pack('hfff', 0x4d30, p, i, d))
    
    def get_voltage_current(self):
        self.dev.flushInput()
        cmd = struct.pack('H', self.CMD_ADC)
        self.dev.write(cmd)

        data = self.dev.read(64)
        data = list(map(int, data.split(b',')))

        return data


##############
if __name__ == "__main__":
    v = Vecicle()
    print(v.get_voltage_current())