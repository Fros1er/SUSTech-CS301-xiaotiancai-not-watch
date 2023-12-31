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
    allow_chars = '0123456789+-*%/^xyz()\n '
    if not all(c in allow_chars for c in dat):
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
            return solve(dat)
    except:
        return b"Error!"


com_name = str(list(serial.tools.list_ports.comports())[0])[:4]
print(f"Connecting to {com_name}...")
com = serial.Serial(com_name, 115200)
print("Connected!")

work = ""


def send_packet(target_addr, _cmd, _msg):
    com.write(struct.pack('b', target_addr) + struct.pack('b', _cmd) + _msg)


while 1:
    work += com.read(64)
    # extract every packet with format: [SERVER A]msg[SERVER B]
    while work.find(b"[SERVER A]") != -1 and work.find(b"[SERVER B]") != -1:
        msg = work[work.find(b"[SERVER A]") + 10:work.find(b"[SERVER B]")]
        work = work[work.find(b"[SERVER B]") + 10:]
        print(f"Received: {msg}")
        if msg[1] == 0x05:  # CALC_REQUEST
            res = calc(msg[2:].decode())
            print(f"Result: {res}")
            send_packet(msg[0], 0x04, res)
    time.sleep(0.1)
