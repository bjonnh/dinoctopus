#  Copyright (c) 2024. Jonathan Bisson
#  SPDX-License-Identifier: MIT
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

from time import sleep

import mido
from mido import Message

# Replace 'sysex_data' with your SysEx command bytes
sysex_prefix = [0x44, 0x49, 0x4e, 0x4f]
device_id = [0x00]
sysex_data = [0x01]

# Create a SysEx message
msg = Message('sysex', data=sysex_prefix+sysex_data)

print(mido.get_output_names())
print(mido.get_input_names())


def uint32_to_ints(value):
    """
    Note that we use this little endian converter, but we cannot send messages bigger than 127 as each 'byte' is 7 bits
    So either we need to come up with a better encoding or we stay with 127

    :param value:
    :return:
    """
    return [int(x) for x in value.to_bytes(4, byteorder='little')]

class CommandMaker:
    def __init__(self, prefix, outport, inport):
        self.prefix = prefix
        self.outport = outport
        self.inport = inport
        self.stop_the_world = False

    def send_command(self, command, data):
        outport.send(Message('sysex', data=self.prefix + [command] + data))

    def dump_matrix(self):
        return self.send_command(0x01, [])

    def set_matrix(self, matrix):
        return self.send_command(0x02, matrix)

    def set_ij(self, i, j, value):
        return self.send_command(0x04, [i, j] + uint32_to_ints(value))

    def get_ij(self, i, j):
        return self.send_command(0x03, [i, j])

    def load(self):
        return self.send_command(0x06, [])

    def save(self):
        return self.send_command(0x05, [])

    def reset(self):
        return self.send_command(0x07, [])

    def ack(self):
        return self.last_sysex() == sysex_prefix + device_id + [0x66, 0x60]

    def nack(self):
        return self.last_sysex() == sysex_prefix + device_id + [0x66, 0x61]

    def last_sysex(self, counts=10):
        while True:
            counts -= 1
            sleep(0.1)
            for msg in self.inport.iter_pending():
                if msg.type == 'sysex':
                    return list(msg.data)
            if self.stop_the_world or counts == 0:
                return None

    def expect_last_sysex_to_eq(self, expected):
        expected = sysex_prefix + device_id + [0x66] + expected
        val = self.last_sysex()
        if val != expected:
            print()
            print(f"   Expected {expected} but got {val}")
        return True

def test_dump_matrix(commandMaker):
    print("Testing DUMP_MATRIX ",end="", flush=True)
    commandMaker.dump_matrix()
    assert len(commandMaker.last_sysex()) == 100 + len(sysex_prefix + device_id + [0x66])
    print("OK")

def test_reset(commandMaker):
    print("Testing RESET ",end="")
    commandMaker.set_matrix([1] * 100)
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([1] * 100)
    commandMaker.reset()
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([0] * 100)
    print("OK")

def test_setij(commandMaker):
    print("Testing SET_IJ",end="")
    commandMaker.reset()
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([0] + [0] * 99)
    commandMaker.set_ij(0, 0, 1)
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([1] + [0] * 99)
    print("OK")


def test_getij(commandMaker):
    print("Testing GET_IJ ",end="")
    commandMaker.reset()
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([0] + [0] * 99)
    for i in range(0, 5):
        for j in range(0, 5):
            commandMaker.get_ij(i, j)
            commandMaker.expect_last_sysex_to_eq(uint32_to_ints(0))
            commandMaker.set_ij(i, j, i*j)
            assert commandMaker.ack()
            commandMaker.get_ij(i, j)
            commandMaker.expect_last_sysex_to_eq(uint32_to_ints(i*j))
    print("OK")

def test_load(commandMaker):
    print("Testing LOAD/SAVE ",end="")
    commandMaker.reset()
    assert commandMaker.ack()
    commandMaker.save()
    assert commandMaker.ack()
    commandMaker.load()
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([0] + [0] * 99)
    for i in range(0, 5):
        for j in range(0, 5):
            commandMaker.set_ij(i, j, (i+1)*(j+1))
            assert commandMaker.ack()
    commandMaker.save()
    assert commandMaker.ack()
    commandMaker.reset()
    assert commandMaker.ack()
    commandMaker.dump_matrix()
    commandMaker.expect_last_sysex_to_eq([0] + [0] * 99)
    commandMaker.load()
    assert commandMaker.ack()
    for i in range(0, 5):
        for j in range(0, 5):
            commandMaker.get_ij(i, j)
            commandMaker.expect_last_sysex_to_eq(uint32_to_ints((i+1)*(j+1)))
    print("OK")


for output_port_name in mido.get_output_names():
    if "DIN" not in output_port_name:
        continue
    if ":1" not in output_port_name: # For now we only allow that on 1
        continue
    print(output_port_name)
    with mido.open_output(output_port_name) as outport:
        with mido.open_input(output_port_name) as inport:
            for msg in inport.iter_pending():
                pass
            print("Ready to start")
            commandMaker = CommandMaker(sysex_prefix + device_id, outport, inport)
            test_dump_matrix(commandMaker)
            test_reset(commandMaker)
            test_setij(commandMaker)
            test_getij(commandMaker)
            test_load(commandMaker)

            commandMaker.reset()
            for i in range(0, 5):
                commandMaker.set_ij(i, i, 1)
                assert commandMaker.ack()
            commandMaker.save()
            assert commandMaker.ack()
