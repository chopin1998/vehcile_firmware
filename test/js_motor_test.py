#!/usr/bin/python3

import time
import struct
import asyncio

import evdev
from vehicle import Vecicle


def test():
    v = Vecicle()

    js = evdev.InputDevice('/dev/input/by-id/usb-MY-POWER_LeWGP-201-event-joystick')

    print(js)

    # left joy
    abs_x = 128
    abs_y = 128
    abs_z = 128
    abs_rz = 128
    abs_hx = 0
    abs_hy = 0
    abs_gas = 0
    abs_brk = 0

    for ev in js.read_loop():
        if ev.type == evdev.ecodes.EV_ABS:
            if ev.code == evdev.ecodes.ABS_X:
                abs_x = ev.value
            elif ev.code == evdev.ecodes.ABS_Y:
                abs_y = ev.value
            elif ev.code == evdev.ecodes.ABS_Z:
                abs_z = ev.value
            elif ev.code == evdev.ecodes.ABS_RZ:
                abs_rz = ev.value
            elif ev.code == evdev.ecodes.ABS_HAT0X:
                abs_hx = ev.value
            elif ev.code == evdev.ecodes.ABS_HAT0Y:
                abs_hy = ev.value
            elif ev.code == evdev.ecodes.ABS_GAS:
                abs_gas = ev.value
            elif ev.code == evdev.ecodes.ABS_BRAKE:
                abs_brk = ev.value
            
        elif ev.type == evdev.ecodes.EV_KEY:
            print(evdev.categorize(ev))
        if ev.type == evdev.ecodes.EV_SYN:
            # print('////\n')
            pass
        # print(evdev.ecodes.EV.get(ev.type))
        # print(abs_x, abs_y, abs_z, abs_rz, abs_hx, abs_hy, abs_gas, abs_brk)

        if abs_x < 128-64:
            if abs_y < 128 - 64:
                d = 'LF'
            elif abs_y > 128 + 64:
                d = 'LB'
            else:
                d = 'L'
        elif abs_x > 128+64:
            if abs_y < 128 - 64:
                d = 'RF'
            elif abs_y > 128 + 64:
                d = 'RB'
            else:
                d = 'R'
        else:
            if abs_y < 128 - 64:
                d = 'F'
            elif abs_y > 128 + 64:
                d = 'B'
            else:
                d = 'STOP'
        
        spd = int(abs_gas / 255 * 25) + 2
        if (d != 'STOP'):
            print(d, spd)
        else:
            if abs_z < 128 - 64:
                d = 'RL'
            elif abs_z > 128 + 64:
                d = 'RR'
        
        if d == 'L':
            v.move_lr(spd)
        elif d == 'R':
            v.move_lr(-spd)
        elif d == 'F':
            v.move_fb(spd)
        elif d == 'B':
            v.move_fb(-spd)
        elif d in ('LF', 'LB', 'RF', 'RB'):
            v.move_diag(d, spd)
        elif d == 'RL':
            v.move_rotate(spd)
        elif d == 'RR':
            v.move_rotate(-spd)
        else:
            v.motors_spd(0,0,0,0)

if __name__ == "__main__":
    test()
