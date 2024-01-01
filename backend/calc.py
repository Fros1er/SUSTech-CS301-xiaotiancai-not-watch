import struct
import time
import serial.tools.list_ports
import re


def solve(func):
    template = f'''
from z3 import *

{'x = Real("x")' if 'x' in func else ''}
{'y = Real("y")' if 'y' in func else ''}
{'z = Real("z")' if 'z' in func else ''}
s = Solver()
'''
    for f in func.split('\n'):
        template += f'\ns.add({f})\n'
    template += f'''
res = ""
if s.check() == sat:
    max_ans = 5
    while s.check() == sat and max_ans > 0:
        max_ans -= 1
        m = s.model()
        {'res += "x=" + str(m[x]) + chr(10)' if 'x' in func else ''}
        {'res += "y=" + str(m[y]) + chr(10)' if 'y' in func else ''}
        {'res += "z=" + str(m[z]) + chr(10)' if 'z' in func else ''}
        res += "--" + chr(10)
        s.add(Or({'x != m[x],' if 'x' in func else ''}{'y != m[y],' if 'y' in func else ''}{'z != m[z],' if 'z' in func else ''}))
else:
    res = "No solution---"

with open('res.txt', 'w') as f:
    f.write(res[:-3])
'''
    exec(template)
    with open('res.txt', 'r') as f:
        ans = f.read()
    return ans


def calc(dat):
    dat = str(dat)
    allow_chars = '0123456789+-*%/^xyz()=\n '
    if not all(c in allow_chars for c in dat):
        for i in range(len(dat)):
            if dat[i] not in allow_chars:
                return f"Invalid char: {dat[i]}".encode()
        return b"Invalid input!"
    # make 1^2 to pow(1, 2)
    dat = re.sub(r'([\dxyz]+)\^(\d+)', r'pow(\1, \2)', dat)
    # make 1x to 1*x
    dat = re.sub(r'([\dxyz])([xyz])', r'\1*\2', dat)
    function_chars = 'xyz'
    try:
        if not any(c in function_chars for c in dat):
            assert '\n' not in dat
            return eval(dat)
        else:
            dat = re.sub(r"=", r"==", dat)
            return solve(dat)
    except:
        return b"Error!"


def binary_calc(dat):
    dat = str(dat)
    allow_chars = '01+-*/%^()\n '
    if not all(c in allow_chars for c in dat):
        return b"Invalid input!"
    dat = re.sub(r'([\dxyz]+)\^(\d+)', r'pow(\1, \2)', dat)
    dat = re.sub(r'(\d+)', r'0b\1', dat)
    try:
        return eval(dat)
    except:
        return b"Error!"


com_name = str(list(serial.tools.list_ports.comports())[0])[:4]
print(f"Connecting to {com_name}...")
com = serial.Serial(com_name, 115200)
print("Connected!")

work = b""


def send_packet(target_addr, _cmd, _msg):
    for i in range(0, len(_msg), 29):
        raw = struct.pack('b', target_addr) + struct.pack('b', _cmd) + str(_msg[i:i+29]).encode() + b'\x00'
        print(f"Sending: {raw}")
        com.write(raw)


while 1:
    work += com.read(52)
    print(work)
    # extract every packet with format: [SERVER A]msg[SERVER B]
    while work.find(b"[Server A]") != -1 and work.find(b"[Server B]") != -1:
        msg = work[work.find(b"[Server A]") + 10:work.find(b"[Server B]")] + b'\x00'
        work = work[work.find(b"[Server B]") + 10:]
        print(f"Received: {msg}")
        try:
            msg = msg[:msg.index(b'\x00')]
            if msg[1] == 0x05:  # CALC_REQUEST
                r = calc(msg[2:].decode())
                print(f"Result: {r}")
                send_packet(msg[0], 0x04, str(r))
            if msg[1] == 0x06:  # CALC_BIN_REQUEST
                r = binary_calc(msg[2:].decode())
                print(f"Result: {r}")
                send_packet(msg[0], 0x07, str(r))
        except:
            pass
    time.sleep(0.1)
