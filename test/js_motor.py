#!/usr/bin/python3

import time
import struct
import asyncio

import evdev
from vehicle import Vecicle


def test():
    v = Vecicle()
    v.motors_spd(0, 0, 0, 0)

    js = evdev.InputDevice('/dev/input/by-id/usb-MY-POWER_LeWGP-201-event-joystick')
    print(js)

    # left joy
    abs_x = 128
    abs_y = 128
    abs_rotate = 128
    abs_rz = 128
    abs_hx = 0
    abs_hy = 0
    abs_xy_gas = 0
    abs_r_gas = 0

    for ev in js.read_loop():
        if ev.type == evdev.ecodes.EV_ABS:
            if ev.code == evdev.ecodes.ABS_X:
                abs_x = ev.value
            elif ev.code == evdev.ecodes.ABS_Y:
                abs_y = ev.value
            elif ev.code == evdev.ecodes.ABS_Z:
                abs_rotate = ev.value
            elif ev.code == evdev.ecodes.ABS_RZ:
                abs_rz = ev.value
            elif ev.code == evdev.ecodes.ABS_HAT0X:
                abs_hx = ev.value
            elif ev.code == evdev.ecodes.ABS_HAT0Y:
                abs_hy = ev.value
            elif ev.code == evdev.ecodes.ABS_BRAKE:
                abs_xy_gas = ev.value
            elif ev.code == evdev.ecodes.ABS_GAS:
                abs_r_gas = ev.value
            
        elif ev.type == evdev.ecodes.EV_KEY:
            print(evdev.categorize(ev))
        if ev.type == evdev.ecodes.EV_SYN:
            # print('////\n')
            pass
        # print(evdev.ecodes.EV.get(ev.type))
        # print(abs_x, abs_y, abs_rotate, abs_rz, abs_hx, abs_hy, abs_xy_gas, abs_r_gas)

        spd_xy = abs_xy_gas / 255 * 1.5 + 1
        spd_r = abs_r_gas / 255 * 1.5 + 1
        x = int( (128 - abs_x) * 15 / 128 * spd_xy )
        y = int( (128 - abs_y) * 15 / 128 * spd_xy )
        r = int((128 - abs_rotate) * 15 // 128 * spd_r)

        v.move_mecanum(x, y, r)

        
        

if __name__ == "__main__":
    test()
